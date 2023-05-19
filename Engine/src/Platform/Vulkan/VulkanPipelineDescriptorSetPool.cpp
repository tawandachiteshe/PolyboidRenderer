#include "boidpch.h"
#include "VulkanPipelineDescriptorSetPool.h"
#include <vulkan/vulkan.hpp>

#include "VkRenderAPI.h"
#include "VulkanPipelineDescriptorSet.h"
#include "Utils/VulkanDevice.h"


namespace Polyboid
{
	VulkanPipelineDescriptorSetPool::VulkanPipelineDescriptorSetPool(const VkRenderAPI* context,
		DescriptorSetPoolSettings settings): m_Context(context), m_Settings(settings)
	{

		auto device = context->GetDevice()->GetVulkanDevice();

		std::vector<vk::DescriptorPoolSize> poolSizes;

		if (m_Settings.UniformBufferCount)
		{
			vk::DescriptorPoolSize poolSize{ vk::DescriptorType::eUniformBuffer, settings.UniformBufferCount };
			poolSizes.emplace_back(poolSize);
		}

		if (m_Settings.CombinedSamplerCount)
		{
			vk::DescriptorPoolSize poolSize{ vk::DescriptorType::eCombinedImageSampler, settings.UniformBufferCount };
			poolSizes.emplace_back(poolSize);
		}

		if (m_Settings.StorageBufferCount)
		{
			vk::DescriptorPoolSize poolSize{ vk::DescriptorType::eStorageBuffer, settings.UniformBufferCount };
			poolSizes.emplace_back(poolSize);
		}

		vk::DescriptorPoolCreateInfo createInfo{};
		//createInfo.flags = vk::DescriptorPoolCreateFlagBits::eUpdateAfterBind;
		createInfo.maxSets = 10;
		createInfo.pPoolSizes = poolSizes.data();
		createInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		auto [result, pool] = device.createDescriptorPool(createInfo);
		vk::resultCheck(result, "Failed to create Desc Pool");

		m_DescPool = pool;

		//device.resetDescriptorPool(m_DescPool);
	}


	Ref<PipelineDescriptorSet> VulkanPipelineDescriptorSetPool::GetDescriptorSet(uint32_t index)
	{
		return m_Sets.at(index);
	}

	std::any VulkanPipelineDescriptorSetPool::GetHandle()
	{
		return m_DescPool;
	}

	void VulkanPipelineDescriptorSetPool::Reset()
	{
		auto device = m_Context->GetDevice()->GetVulkanDevice();
		device.resetDescriptorPool(m_DescPool);
	}

	void VulkanPipelineDescriptorSetPool::Destroy()
	{
		auto device = m_Context->GetDevice()->GetVulkanDevice();
		device.destroyDescriptorPool(m_DescPool);
	}

	DescriptorSetPoolSettings VulkanPipelineDescriptorSetPool::GetDescPoolSettings()
	{
		return m_Settings;
	}

	vk::DescriptorPool VulkanPipelineDescriptorSetPool::GetVulkanHandle()
	{
		return m_DescPool;
	}
}
