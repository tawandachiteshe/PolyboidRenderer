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

		//device.resetDescriptorPool(m_DescPool);
		Init();
	}

	void VulkanPipelineDescriptorSetPool::Init()
	{
		const auto device = VkRenderAPI::GetVulkanDevice();

		if (m_Settings.UniformBufferCount)
		{
			vk::DescriptorPoolSize poolSize{ vk::DescriptorType::eUniformBuffer, m_Settings.UniformBufferCount };
			m_PoolSizes.emplace_back(poolSize);
		}

		if (m_Settings.CombinedSamplerCount)
		{
			vk::DescriptorPoolSize poolSize{ vk::DescriptorType::eCombinedImageSampler, m_Settings.UniformBufferCount };
			m_PoolSizes.emplace_back(poolSize);
		}

		if (m_Settings.StorageBufferCount)
		{
			vk::DescriptorPoolSize poolSize{ vk::DescriptorType::eStorageBuffer, m_Settings.UniformBufferCount };
			m_PoolSizes.emplace_back(poolSize);
		}

		if (m_Settings.StorageBufferCount)
		{
			vk::DescriptorPoolSize poolSize{ vk::DescriptorType::eStorageTexelBuffer, m_Settings.UniformBufferCount };
			m_PoolSizes.emplace_back(poolSize);
		}


		if (m_Settings.UniformBufferCount)
		{
			vk::DescriptorPoolSize poolSize{ vk::DescriptorType::eUniformTexelBuffer, m_Settings.UniformBufferCount };
			m_PoolSizes.emplace_back(poolSize);
		}



		vk::DescriptorPoolCreateInfo createInfo{};
		//createInfo.flags = vk::DescriptorPoolCreateFlagBits::eUpdateAfterBind;
		createInfo.maxSets = m_Settings.DescriptorCount;
		//createInfo.flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet;
		createInfo.pPoolSizes = m_PoolSizes.data();
		createInfo.poolSizeCount = static_cast<uint32_t>(m_PoolSizes.size());
		auto [result, pool] = device.createDescriptorPool(createInfo);
		vk::resultCheck(result, "Failed to create Desc Pool");

		m_DescPool = pool;
	}


	void VulkanPipelineDescriptorSetPool::Recreate()
	{
		Destroy();
		Init();

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
		const auto device = m_Context->GetDevice()->GetVulkanDevice();
		device.resetDescriptorPool(m_DescPool);
	}

	void VulkanPipelineDescriptorSetPool::Destroy()
	{
		
		const auto device = m_Context->GetDevice()->GetVulkanDevice();
		device.destroyDescriptorPool(m_DescPool);
		m_DescPool = nullptr;
		m_PoolSizes.clear();
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
