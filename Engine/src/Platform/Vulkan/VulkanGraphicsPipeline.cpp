#include "boidpch.h"
#include "VulkanGraphicsPipeline.h"

#include <spdlog/spdlog.h>

#include "VulkanPipelineDescriptorSet.h"
#include "VulkanPipelineDescriptorSetPool.h"
#include "VulkanRenderPass.h"
#include "VulkanShader.h"
#include "Engine/Renderer/VertexBufferArray.h"
#include "Utils/VulkanDevice.h"


namespace Polyboid
{


	VulkanGraphicsPipeline::VulkanGraphicsPipeline(const VkRenderAPI* context): m_Context(context)
	{
	}

	void VulkanGraphicsPipeline::Bake()
	{
		auto device = m_Context->GetDevice()->GetVulkanDevice();


		auto vulkanVtxShader = std::reinterpret_pointer_cast<VulkanShader>(m_Shaders.at(ShaderType::Vertex));
		auto vulkanFragShader = std::reinterpret_pointer_cast<VulkanShader>(m_Shaders.at(ShaderType::Fragment));

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
			m_PushConstantRanges.insert(std::end(m_PushConstantRanges),
				vulkanVtxShader->GetPushContantRange().begin(), vulkanVtxShader->GetPushContantRange().end());
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
				m_Bindings[setIndex].insert(m_Bindings[0].end(), binding.begin(), binding.end());
			}


		}


		for (const auto& [set,binding] : m_Bindings)
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



		vk::GraphicsPipelineCreateInfo createInfo{};
		vk::PipelineVertexInputStateCreateInfo vtxInput{};
		vtxInput.vertexAttributeDescriptionCount = 0;
		vtxInput.vertexBindingDescriptionCount = 0;

		const auto colorStateInfo = m_BlendState.GetVulkanInfo();
		const auto depthStateInfo = m_DepthStencil.GetVulkanInfo();
		const auto multiSampleStateInfo = m_MultiSampleState.GetVulkanInfo();
		const auto rasterizerStateInfo = m_RasterizerState.GetVulkanInfo();

		if (m_VertexInput)
		{
			vtxInput = m_VertexInput->GetVulkanInfo();
		}


		vk::PipelineInputAssemblyStateCreateInfo vtxInputAssState{};
		vtxInputAssState.primitiveRestartEnable = false;
		vtxInputAssState.topology = vk::PrimitiveTopology::eTriangleList;
		createInfo.pInputAssemblyState = &vtxInputAssState;
		createInfo.pVertexInputState = &vtxInput;


		createInfo.pColorBlendState = &colorStateInfo;
		createInfo.pDepthStencilState = &depthStateInfo;
		createInfo.pMultisampleState = &multiSampleStateInfo;
		createInfo.pDynamicState = &rasterizerStateInfo.m_CreateDynamicStateInfo;
		createInfo.pRasterizationState = &rasterizerStateInfo.m_CreateRasterizeInfo;
		createInfo.pViewportState = &rasterizerStateInfo.m_ViewportCreateInfo;
		createInfo.layout = pipelineLayout;
		createInfo.pStages = m_Stages.data();
		createInfo.stageCount = static_cast<uint32_t>(m_Stages.size());
		createInfo.renderPass = m_RenderPass->GetHandle();
		createInfo.subpass = 0;

	
		auto [graphicsPipelineResult, gfxPipeline] = device.createGraphicsPipeline(nullptr, createInfo);
		vk::resultCheck(graphicsPipelineResult, "Failed to create");

		m_Pipeline = gfxPipeline;
		m_PipelineLayout = pipelineLayout;
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

	std::shared_ptr<Shader> VulkanGraphicsPipeline::GetShader(const ShaderType& type) const
	{
		if (!m_Shaders.contains(type))
		{
			__debugbreak();
			spdlog::info("Shader not found");
		}

		return m_Shaders.at(type);
	}

	const PipelineState::ShaderMap& VulkanGraphicsPipeline::GetShaders() const
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
		m_RenderPass = std::reinterpret_pointer_cast<VulkanRenderPass>(renderPass);
	}

	Ref<RenderPass> VulkanGraphicsPipeline::GetRenderPass()
	{
		return std::reinterpret_pointer_cast<RenderPass>(m_RenderPass);
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
		m_VertexInput = std::reinterpret_pointer_cast<VulkanVertexBufferArray>(va);
	}


	Ref<VertexBufferArray> VulkanGraphicsPipeline::GetVertexArray()
	{
		return m_VertexInput;
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

	void VulkanGraphicsPipeline::AllocateDescSetsFromShaders(const Ref<PipelineDescriptorSetPool>& descPool, uint32_t setBinding)
	{
		m_DescPool = std::reinterpret_pointer_cast<VulkanPipelineDescriptorSetPool>(descPool);
		auto device = m_Context->GetDevice()->GetVulkanDevice();


		vk::DescriptorSetLayout vulkanLayout = m_SetIndexWithLayout[setBinding];
		vk::DescriptorSetAllocateInfo allocateInfo{};
		allocateInfo.descriptorPool = m_DescPool->GetVulkanHandle();
		allocateInfo.descriptorSetCount = m_DescPool->GetDescPoolSettings().ImageCount; //PER each frame

		vk::DescriptorSetLayout vulkanLayouts[] = { vulkanLayout, vulkanLayout, vulkanLayout};
		allocateInfo.pSetLayouts = vulkanLayouts;

		auto [allocResult, allocSets] = device.allocateDescriptorSets(allocateInfo);
		vk::resultCheck(allocResult, "Failed to create Desc Sets");

		std::vector<Ref<PipelineDescriptorSet>> m_PipeDescSets;

		for (auto set : allocSets)
		{
			auto descSet = std::reinterpret_pointer_cast<PipelineDescriptorSet>(std::make_shared<VulkanPipelineDescriptorSet>(set, m_PipelineLayout,m_DescWriteMap[setBinding]));
			m_PipeDescSets.push_back(descSet);
		}

		m_Sets[setBinding] = m_PipeDescSets;


	}

	std::vector<Ref<PipelineDescriptorSet>> VulkanGraphicsPipeline::GetDescriptorSets(uint32_t set)
	{
		return m_Sets.at(set);
	}


	void VulkanGraphicsPipeline::Destroy()
	{
		auto device = m_Context->GetDevice()->GetVulkanDevice();
		device.destroyPipelineLayout(m_PipelineLayout);

		for (auto& layout : m_DescriptorSetLayouts)
		{
			device.destroyDescriptorSetLayout(layout);
		}
		
		device.destroyPipeline(m_Pipeline);
		
	}

	std::any VulkanGraphicsPipeline::GetHandle()
	{
		return m_Pipeline;
	}
}
