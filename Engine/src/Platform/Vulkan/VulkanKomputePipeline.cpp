﻿#include "boidpch.h"
#include "VulkanKomputePipeline.h"
#include <vulkan/vulkan.hpp>

#include "VkRenderAPI.h"
#include "VulkanPipelineDescriptorSet.h"
#include "VulkanPipelineDescriptorSetPool.h"
#include "VulkanShader.h"
#include "Engine/Renderer/RenderCommand.h"
#include <ranges>

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
		m_ResourceRegistry = m_ComputeShader->GetShaderResourceType();


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
		m_ResourceRegistry.clear();
	}

	void VulkanKomputePipeline::Recreate()
	{
		Destroy();
		m_DescPool->Recreate();
		Init();

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

			for (auto [binding, texture] : m_TextureSets3D[set])
			{
				BindTexture3D(binding, texture, set);
			}

			for (auto [binding, texture] : m_Image[set])
			{
				BindImage2D(binding, texture, set);
			}

			for (auto [binding, texture] : m_StorageTexel[set])
			{
				BindTexelStorageBuffer(binding, texture, set);
			}

			for (auto [binding, texture] : m_UniformTexel[set])
			{
				BindTexelUniformBuffer(binding, texture, set);
			}

			WriteSetResourceBindings(set);
		}
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

		m_Image[setBinding][binding] = (bufferSet);
	}

	void VulkanKomputePipeline::BindTexelUniformBuffer(uint32_t binding, const Ref<TexelUniformBuffer>& bufferSet,
		uint32_t setBinding)
	{
		for (uint32_t i = 0; i < RenderCommand::GetMaxFramesInFlight(); ++i)
		{
			m_Sets[setBinding].at(i)->WriteTexelUniformBuffer(binding, bufferSet);
		}

		m_UniformTexel[setBinding][binding] = (bufferSet);
	}

	void VulkanKomputePipeline::BindTexelStorageBuffer(uint32_t binding, const Ref<TexelStorageBuffer>& bufferSet,
		uint32_t setBinding)
	{
		for (uint32_t i = 0; i < RenderCommand::GetMaxFramesInFlight(); ++i)
		{
			m_Sets[setBinding].at(i)->WriteTexelStorageBuffer(binding, bufferSet);
		}

		m_StorageTexel[setBinding][binding] = (bufferSet);
	}

	void VulkanKomputePipeline::BindTexture3D(uint32_t binding, const Ref<Texture3D>& texture, uint32_t setBinding)
	{
		for (uint32_t i = 0; i < RenderCommand::GetMaxFramesInFlight(); ++i)
		{
			m_Sets[setBinding].at(i)->WriteTexture3D(binding, texture);
		}

		m_TextureSets3D[setBinding][binding] = (texture);
	}

	void VulkanKomputePipeline::WriteSetResourceBindings(uint32_t set)
	{
		for (uint32_t i = 0; i < RenderCommand::GetMaxFramesInFlight(); ++i)
		{
			m_Sets[set].at(i)->Commit();
		}
	}

	RenderResourceType VulkanKomputePipeline::GetRenderResourceType()
	{
		return RenderResourceType::None;
	}

	void VulkanKomputePipeline::BindResource(const std::string& name, const Ref<RenderResource>& resource)
	{
		const auto resourceInfo = m_ResourceRegistry.at(name);
		switch (resourceInfo.ResourceType)
		{
		case RenderResourceType::Image: BindImage2D(resourceInfo.Binding, resource.As<Image2D>(), resourceInfo.Set);
			break;
		case RenderResourceType::TexelStorageBuffer: BindTexelStorageBuffer(
			resourceInfo.Binding, resource.As<TexelStorageBuffer>(), resourceInfo.Set);
			break;
		case RenderResourceType::TexelUniformBuffer: BindTexelUniformBuffer(
			resourceInfo.Binding, resource.As<TexelUniformBuffer>(), resourceInfo.Set);
			break;
		case RenderResourceType::Texture2D: BindTexture2D(resourceInfo.Binding, resource.As<Texture2D>(),
			resourceInfo.Set);
			break;
		case RenderResourceType::Texture3D: BindTexture3D(resourceInfo.Binding, resource.As<Texture3D>(),
			resourceInfo.Set);
			break;
		case RenderResourceType::StorageBuffer: BindStorageBufferSet(resourceInfo.Binding,
			resource.As<StorageBufferSet>(), resourceInfo.Set);
			break;
		case RenderResourceType::UniformBuffer: BindUniformBufferSet(resourceInfo.Binding,
			resource.As<UniformBufferSet>(), resourceInfo.Set);
			break;
		case RenderResourceType::None: __debugbreak();
			break;
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
