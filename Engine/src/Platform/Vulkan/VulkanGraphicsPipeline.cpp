#include "boidpch.h"
#include "VulkanGraphicsPipeline.h"

#include <spdlog/spdlog.h>

#include "VulkanPipelineDescriptorSet.h"
#include "VulkanPipelineDescriptorSetPool.h"
#include "VulkanRenderPass.h"
#include "VulkanShader.h"
#include "Engine/Renderer/BufferSet.h"
#include "Engine/Renderer/RenderCommand.h"
#include "Engine/Renderer/VertexBufferArray.h"
#include "Utils/VulkanDevice.h"


namespace Polyboid
{


	VulkanGraphicsPipeline::VulkanGraphicsPipeline(const VkRenderAPI* context): m_Context(context)
	{

		m_DescPool = CreateRef<VulkanPipelineDescriptorSetPool>(context, DescriptorSetPoolSettings{ RenderCommand::GetMaxFramesInFlight() });
	}

	void VulkanGraphicsPipeline::Bake()
	{
	

		auto vulkanVtxShader = m_Shaders.at(ShaderType::Vertex).As<VulkanShader>(); 
		auto vulkanFragShader = m_Shaders.at(ShaderType::Fragment).As<VulkanShader>();

		m_Stages.emplace_back(vulkanVtxShader->GetVulkanPipelineStageInfo());
		m_Stages.emplace_back(vulkanFragShader->GetVulkanPipelineStageInfo());


		for (auto& [setIndex, writeDesc] : vulkanVtxShader->GetDescWriteMap())
		{
			m_DescWriteMap[setIndex].insert(writeDesc.begin(), writeDesc.end());
		}

		for (auto& [setIndex, writeDesc] : vulkanFragShader->GetDescWriteMap())
		{
			m_DescWriteMap[setIndex].insert(writeDesc.begin(), writeDesc.end());
		}


		if (!vulkanVtxShader->GetPushContantRange().empty())
		{
			m_PushConstantRanges.push_back(vulkanVtxShader->GetPushContantRange().at(0));
		}

		if (!vulkanFragShader->GetPushContantRange().empty())
		{
			m_PushConstantRanges.insert(std::end(m_PushConstantRanges),
				vulkanFragShader->GetPushContantRange().begin(), vulkanFragShader->GetPushContantRange().end());
		}



		if (!vulkanVtxShader->GetVulkanDescriptorBindings().empty())
		{
			const auto vtxShaderBindings = vulkanVtxShader->GetVulkanDescriptorBindings();


			for (const auto& [setIndex, binding] : vtxShaderBindings)
			{
				m_Bindings[setIndex].insert(m_Bindings[setIndex].end(), binding.begin(), binding.end());
			}

		}

		if (!vulkanFragShader->GetVulkanDescriptorBindings().empty())
		{
			const auto fragBindings = vulkanFragShader->GetVulkanDescriptorBindings();

			for (const auto& [setIndex, binding] : fragBindings)
			{
				m_Bindings[setIndex].insert(m_Bindings[setIndex].end(), binding.begin(), binding.end());
			}

		}

		Init();

	}

	void VulkanGraphicsPipeline::Init()
	{
		auto device = m_Context->GetDevice()->GetVulkanDevice();

		for (const auto& [set, binding] : m_Bindings)
		{
			vk::DescriptorSetLayoutCreateInfo layoutCreateInfo{};
			layoutCreateInfo.bindingCount = static_cast<uint32_t>(binding.size());
			layoutCreateInfo.pBindings = binding.data();
			//Used to create layout;

			auto [descSetResult, descLayout] = device.createDescriptorSetLayout(layoutCreateInfo);
			vk::resultCheck(descSetResult, "Failed to create layout");
			m_SetIndexWithLayout[set] = descLayout;
			m_DescriptorSetLayouts.emplace_back(descLayout);
		}

	



		vk::PipelineLayoutCreateInfo pipelineCreateInfo{};
		pipelineCreateInfo.pPushConstantRanges = m_PushConstantRanges.data();
		pipelineCreateInfo.pushConstantRangeCount = static_cast<uint32_t>(m_PushConstantRanges.size());
		pipelineCreateInfo.pSetLayouts = m_DescriptorSetLayouts.data();
		pipelineCreateInfo.setLayoutCount = static_cast<uint32_t>(m_DescriptorSetLayouts.size());

		auto [layoutResult, pipelineLayout] = device.createPipelineLayout(pipelineCreateInfo);
		vk::resultCheck(layoutResult, "Failed to create pipeline layout");
		m_PipelineLayout = pipelineLayout;


		vk::PipelineVertexInputStateCreateInfo vtxInput{};
		vtxInput.vertexAttributeDescriptionCount = 0;
		vtxInput.vertexBindingDescriptionCount = 0;

		const auto colorStateInfo = m_BlendState.GetVulkanInfo();
		const auto depthStateInfo = m_DepthStencil.GetVulkanInfo();
		const auto multiSampleStateInfo = m_MultiSampleState.GetVulkanInfo();
		const auto rasterizerStateInfo = m_RasterizerState.GetVulkanInfo();

		if (m_VertexInput.Get())
		{
			vtxInput = m_VertexInput->GetVulkanInfo();
		}


		vk::PipelineInputAssemblyStateCreateInfo vtxInputAssState{};
		vtxInputAssState.primitiveRestartEnable = false;
		vtxInputAssState.topology = vk::PrimitiveTopology::eTriangleList;
		m_CreatePipelineInfo.pInputAssemblyState = &vtxInputAssState;
		m_CreatePipelineInfo.pVertexInputState = &vtxInput;
		m_CreatePipelineInfo.pColorBlendState = &colorStateInfo;
		m_CreatePipelineInfo.pDepthStencilState = &depthStateInfo;
		m_CreatePipelineInfo.pMultisampleState = &multiSampleStateInfo;
		m_CreatePipelineInfo.pDynamicState = &rasterizerStateInfo.m_CreateDynamicStateInfo;
		m_CreatePipelineInfo.pRasterizationState = &rasterizerStateInfo.m_CreateRasterizeInfo;
		m_CreatePipelineInfo.pViewportState = &rasterizerStateInfo.m_ViewportCreateInfo;
		m_CreatePipelineInfo.layout = pipelineLayout;
		m_CreatePipelineInfo.pStages = m_Stages.data();
		m_CreatePipelineInfo.stageCount = static_cast<uint32_t>(m_Stages.size());
		m_CreatePipelineInfo.renderPass = m_RenderPass->GetHandle();
		m_CreatePipelineInfo.subpass = 0;


		auto [graphicsPipelineResult, gfxPipeline] = device.createGraphicsPipeline(nullptr, m_CreatePipelineInfo);
		vk::resultCheck(graphicsPipelineResult, "Failed to create");

		m_Pipeline = gfxPipeline;
		m_PipelineLayout = pipelineLayout;
		
	}

	void VulkanGraphicsPipeline::Recreate()
	{
		Destroy();
		m_RenderPass->Recreate();
	//	m_VertexInput->Recreate();
		m_DescPool->Recreate();
		Bake();

		for (const auto& binding : m_Sets | std::views::keys)
		{
			m_Sets[binding] = AllocateDescriptorSets(binding);
		}

		for (const auto& set : m_Sets | std::views::keys)
		{
			for (auto [binding, buffer] : m_UniformBufferSets[set])
			{
				buffer->Recreate();
				BindUniformBufferSet(binding, buffer, set);
			}


			for (auto [binding, buffer] : m_StorageBufferSets[set])
			{
				buffer->Recreate();
				BindStorageBufferSet(binding, buffer, set);
			}


			for (auto [binding, texture] : m_TextureSets[set])
			{
				BindTexture2D(binding, texture, set);
			}



			WriteSetResourceBindings(set);
		}

	}

	void VulkanGraphicsPipeline::SetShader(const ShaderType& type, const Ref<Shader>& shader)
	{
		if (m_Shaders.contains(type))
		{
			__debugbreak();
		}

		m_Shaders[type] = shader;
	
		
	}

	void VulkanGraphicsPipeline::SetShader(const Ref<Shader>& shader)
	{
		SetShader(shader->GetType(), shader);
	}

	Ref<Shader> VulkanGraphicsPipeline::GetShader(const ShaderType& type) const
	{
		if (!m_Shaders.contains(type))
		{
			__debugbreak();
			spdlog::info("Shader not found");
		}

		return m_Shaders.at(type).As<Shader>();
	}

	const GraphicsPipeline::ShaderMap& VulkanGraphicsPipeline::GetShaders() const
	{
		return m_Shaders;
	}

	void VulkanGraphicsPipeline::SetBlendState(const BlendState& blendState)
	{
	}

	BlendState& VulkanGraphicsPipeline::GetBlendState()
	{
		return m_BlendState;
	}

	void VulkanGraphicsPipeline::SetRasterizerState(const RasterizerState& rasterizerState)
	{
		m_RasterizerState = dynamic_cast<const VulkanRasterizerState&>(rasterizerState);
	}

	RasterizerState& VulkanGraphicsPipeline::GetRasterizerState()
	{
		return m_RasterizerState;
	}

	void VulkanGraphicsPipeline::SetRenderPass(const Ref<RenderPass>& renderPass)
	{
		m_RenderPass = renderPass.As<VulkanRenderPass>(); 
	}

	Ref<RenderPass> VulkanGraphicsPipeline::GetRenderPass()
	{
		return m_RenderPass.As<RenderPass>();
	}

	void VulkanGraphicsPipeline::SetDepthStencilState(const DepthStencilState& depthStencilState)
	{
		m_DepthStencil = dynamic_cast<const VulkanDepthStencilState&>(depthStencilState);
	}

	DepthStencilState& VulkanGraphicsPipeline::GetDepthStencilState()
	{
		return m_DepthStencil;
	}

	void VulkanGraphicsPipeline::SetVertexArray(const Ref<VertexBufferArray>& va)
	{
		m_VertexInput = va.As<VulkanVertexBufferArray>();
	}


	Ref<VertexBufferArray> VulkanGraphicsPipeline::GetVertexArray()
	{
		return m_VertexInput.As<VertexBufferArray>();
		//dynamic_cast<std::nullptr_t&>(nullptr);
	}

	void VulkanGraphicsPipeline::SetMultiSampleState(const MultiSampleState& multiSampleState)
	{
		m_MultiSampleState = dynamic_cast<const VulkanMultiSampleState&>(multiSampleState);
	}

	MultiSampleState& VulkanGraphicsPipeline::GetMutltiSampleState()
	{
		return m_MultiSampleState;
	}

	void VulkanGraphicsPipeline::SetGraphicsShaders(const GraphicsShaders& shaders)
	{
		auto& [vtxShader, fragShader] = shaders;

		m_Shaders[vtxShader->GetType()] = vtxShader;
		m_Shaders[fragShader->GetType()] = fragShader;
	}

	vk::PipelineLayout VulkanGraphicsPipeline::GetPipelineLayout()
	{
		return m_PipelineLayout;
	}

	VulkanGraphicsPipeline::~VulkanGraphicsPipeline()
	{
	}

	void VulkanGraphicsPipeline::Bind()
	{
	}

	void VulkanGraphicsPipeline::UnBind()
	{
	}

	std::vector<Ref<PipelineDescriptorSet>> VulkanGraphicsPipeline::AllocateDescriptorSets(uint32_t setBinding)
	{
		auto device = m_Context->GetDevice()->GetVulkanDevice();

		vk::DescriptorSetLayout vulkanLayout = m_SetIndexWithLayout[setBinding];
		vk::DescriptorSetAllocateInfo allocateInfo{};
		allocateInfo.descriptorPool = m_DescPool->GetVulkanHandle();
		allocateInfo.descriptorSetCount = m_DescPool->GetDescPoolSettings().ImageCount; //PER each frame

		vk::DescriptorSetLayout vulkanLayouts[] = { vulkanLayout, vulkanLayout, vulkanLayout};
		allocateInfo.pSetLayouts = vulkanLayouts;

		auto [allocResult, allocSets] = device.allocateDescriptorSets(allocateInfo);
		vk::resultCheck(allocResult, "Failed to create Desc Sets");

		

		for (auto set : allocSets)
		{
			auto descSet = CreateRef<VulkanPipelineDescriptorSet>(set, m_PipelineLayout, m_DescWriteMap[setBinding]).As<PipelineDescriptorSet>();
			m_PipeDescSets.push_back(descSet);
		}

		m_Sets[setBinding] = m_PipeDescSets;


		return m_Sets.at(setBinding);

	}

	

	std::vector<Ref<PipelineDescriptorSet>> VulkanGraphicsPipeline::GetDescriptorSets(uint32_t set)
	{
		return m_Sets.at(set);
	}

	void VulkanGraphicsPipeline::BindUniformBufferSet(uint32_t binding, const Ref<UniformBufferSet>& bufferSet,
		uint32_t setBinding)
	{
		for (uint32_t i = 0; i < RenderCommand::GetMaxFramesInFlight(); ++i)
		{
			m_Sets[setBinding].at(i)->WriteUniformBuffer(binding, bufferSet->Get(i));
		}

		m_UniformBufferSets[setBinding][binding] = (bufferSet);
	}

	void VulkanGraphicsPipeline::BindStorageBufferSet(uint32_t binding, const Ref<StorageBufferSet>& bufferSet,
		uint32_t setBinding)
	{
		for (uint32_t i = 0; i < RenderCommand::GetMaxFramesInFlight(); ++i)
		{
			m_Sets[setBinding].at(i)->WriteStorageBuffer(binding, bufferSet->Get(i));
		}

		m_StorageBufferSets[setBinding][binding] = (bufferSet);
	}

	void VulkanGraphicsPipeline::BindTexture2D(uint32_t binding, const Ref<Texture>& bufferSet, uint32_t setBinding)
	{
		for (uint32_t i = 0; i < RenderCommand::GetMaxFramesInFlight(); ++i)
		{
			m_Sets[setBinding].at(i)->WriteTexture2D(binding, bufferSet);
		}

		m_TextureSets[setBinding][binding] = (bufferSet);
	}

	void VulkanGraphicsPipeline::WriteSetResourceBindings(uint32_t set)
	{
		for (uint32_t i = 0; i < RenderCommand::GetMaxFramesInFlight(); ++i)
		{
			m_Sets[set].at(i)->Commit();
		}
	}


	void VulkanGraphicsPipeline::Destroy()
	{
		auto device = m_Context->GetDevice()->GetVulkanDevice();

		for (auto& layout : m_DescriptorSetLayouts)
		{
			device.destroyDescriptorSetLayout(layout);
		}
		m_DescPool->Destroy();
		device.destroyPipelineLayout(m_PipelineLayout);
		device.destroyPipeline(m_Pipeline);
		m_DescWriteMap.clear();
		m_PushConstantRanges.clear();
		m_DescriptorSetLayouts.clear();
		m_Bindings.clear();
		m_Stages.clear();
		m_PipeDescSets.clear();
		
	}

	std::any VulkanGraphicsPipeline::GetHandle()
	{
		return m_Pipeline;
	}
}
