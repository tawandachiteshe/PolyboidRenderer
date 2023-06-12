#include "boidpch.h"
#include "VulkanKomputePipeline.h"
#include <vulkan/vulkan.hpp>

#include "VkRenderAPI.h"
#include "VulkanPipelineDescriptorSet.h"
#include "VulkanPipelineDescriptorSetPool.h"
#include "VulkanShader.h"
#include "Engine/Renderer/RenderCommand.h"

namespace Polyboid
{
	VulkanKomputePipeline::VulkanKomputePipeline()
	{
		m_DescPool = CreateRef<VulkanPipelineDescriptorSetPool>(reinterpret_cast<VkRenderAPI*>(RenderAPI::Get()), DescriptorSetPoolSettings{ RenderCommand::GetMaxFramesInFlight() });
	}

	void VulkanKomputePipeline::Init()
	{

		const auto device = VkRenderAPI::GetVulkanDevice();

		m_Stages.emplace_back(m_ComputeShader->GetVulkanPipelineStageInfo());


		for (auto& [setIndex, writeDesc] : m_ComputeShader->GetDescWriteMap())
		{
			m_DescWriteMap[setIndex].insert(writeDesc.begin(), writeDesc.end());
		}

		for (auto& [setIndex, writeDesc] : m_ComputeShader->GetDescWriteMap())
		{
			m_DescWriteMap[setIndex].insert(writeDesc.begin(), writeDesc.end());
		}


		if (!m_ComputeShader->GetPushContantRange().empty())
		{
			m_PushConstantRanges.push_back(m_ComputeShader->GetPushContantRange().at(0));
		}



		if (!m_ComputeShader->GetVulkanDescriptorBindings().empty())
		{
			const auto vtxShaderBindings = m_ComputeShader->GetVulkanDescriptorBindings();


			for (const auto& [setIndex, binding] : vtxShaderBindings)
			{
				m_Bindings[setIndex].insert(m_Bindings[setIndex].end(), binding.begin(), binding.end());
			}

		}

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
		pipelineCreateInfo.setPushConstantRanges(m_PushConstantRanges);
		pipelineCreateInfo.setSetLayouts(m_DescriptorSetLayouts);

		auto [layoutResult, pipelineLayout] = device.createPipelineLayout(pipelineCreateInfo);
		vk::resultCheck(layoutResult, "Failed to create pipeline layout");
		m_PipelineLayout = pipelineLayout;

		vk::ComputePipelineCreateInfo komputeInfo{};
		komputeInfo.setLayout(m_PipelineLayout);
		komputeInfo.setStage(m_Stages.at(0));

		auto [result, pipeline] = device.createComputePipeline(nullptr, komputeInfo);
		vk::resultCheck(result, "Failed to create kompute pipeline");

		m_Pipeline = pipeline;
	}

	void VulkanKomputePipeline::SetComputeShader(const Ref<Shader>& computeShader)
	{
		m_ComputeShader = computeShader.As<VulkanShader>();
	}

	void VulkanKomputePipeline::Destroy()
	{
		const auto device = VkRenderAPI::GetVulkanDevice();

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

	void VulkanKomputePipeline::Recreate()
	{
		Destroy();
		Init();
	}

	vk::PipelineLayout VulkanKomputePipeline::GetPipelineLayout()
	{
		return m_PipelineLayout;
	}

	std::any VulkanKomputePipeline::GetHandle()
	{
		return m_Pipeline;
	}

	std::vector<Ref<PipelineDescriptorSet>> VulkanKomputePipeline::AllocateDescriptorSets(uint32_t setBinding)
	{
		const auto device = VkRenderAPI::GetVulkanDevice();

		vk::DescriptorSetLayout vulkanLayout = m_SetIndexWithLayout[setBinding];
		vk::DescriptorSetAllocateInfo allocateInfo{};
		allocateInfo.descriptorPool = m_DescPool->GetVulkanHandle();
		allocateInfo.descriptorSetCount = m_DescPool->GetDescPoolSettings().ImageCount; //PER each frame

		vk::DescriptorSetLayout vulkanLayouts[] = { vulkanLayout, vulkanLayout, vulkanLayout };
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

	std::vector<Ref<PipelineDescriptorSet>> VulkanKomputePipeline::GetDescriptorSets(uint32_t set)
	{
		return m_Sets.at(set);
	}

	void VulkanKomputePipeline::BindUniformBufferSet(uint32_t binding, const Ref<UniformBufferSet>& bufferSet,
		uint32_t setBinding)
	{
		for (uint32_t i = 0; i < RenderCommand::GetMaxFramesInFlight(); ++i)
		{
			m_Sets[setBinding].at(i)->WriteUniformBuffer(binding, bufferSet->Get(i));
		}

		m_UniformBufferSets[setBinding][binding] = (bufferSet);
	}

	void VulkanKomputePipeline::BindStorageBufferSet(uint32_t binding, const Ref<StorageBufferSet>& bufferSet,
		uint32_t setBinding)
	{
		for (uint32_t i = 0; i < RenderCommand::GetMaxFramesInFlight(); ++i)
		{
			m_Sets[setBinding].at(i)->WriteStorageBuffer(binding, bufferSet->Get(i));
		}

		m_StorageBufferSets[setBinding][binding] = (bufferSet);
	}

	void VulkanKomputePipeline::BindTexture2D(uint32_t binding, const Ref<Texture2D>& bufferSet, uint32_t setBinding)
	{
		for (uint32_t i = 0; i < RenderCommand::GetMaxFramesInFlight(); ++i)
		{
			m_Sets[setBinding].at(i)->WriteTexture2D(binding, bufferSet);
		}

		m_TextureSets[setBinding][binding] = (bufferSet);
	}

	void VulkanKomputePipeline::BindImage2D(uint32_t binding, const Ref<Image2D>& bufferSet, uint32_t setBinding)
	{
		for (uint32_t i = 0; i < RenderCommand::GetMaxFramesInFlight(); ++i)
		{
			m_Sets[setBinding].at(i)->WriteImage2D(binding, bufferSet);
		}

		m_ImageSets[setBinding][binding] = (bufferSet);
	}

	void VulkanKomputePipeline::WriteSetResourceBindings(uint32_t set)
	{
		for (uint32_t i = 0; i < RenderCommand::GetMaxFramesInFlight(); ++i)
		{
			m_Sets[set].at(i)->Commit();
		}
	}

	void VulkanKomputePipeline::Bake()
	{
		Init();
	}

	VulkanKomputePipeline::~VulkanKomputePipeline()
	{
	}
}
