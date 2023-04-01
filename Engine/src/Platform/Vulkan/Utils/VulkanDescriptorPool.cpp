#include "boidpch.h"
#include "VulkanDescriptorPool.h"

#include "VulkanDevice.h"
#include "Platform/Vulkan/VkRenderAPI.h"


namespace Polyboid
{
	VulkanDescriptorPool::VulkanDescriptorPool(const VkRenderAPI* m_Context): m_Context(m_Context)
	{
        auto device = m_Context->GetDevice()->GetDevice();

        vk::DescriptorPoolSize pool_sizes[] =
        {
            { vk::DescriptorType::eSampler, 1000 },
            { vk::DescriptorType::eCombinedImageSampler, 1000 },
            { vk::DescriptorType::eSampledImage, 1000 },
            { vk::DescriptorType::eStorageImage, 1000 },
            { vk::DescriptorType::eUniformTexelBuffer, 1000 },
            { vk::DescriptorType::eStorageTexelBuffer, 1000 },
            { vk::DescriptorType::eUniformBuffer, 1000 },
            { vk::DescriptorType::eStorageBuffer, 1000 },
            { vk::DescriptorType::eUniformBufferDynamic, 1000 },
            { vk::DescriptorType::eStorageBufferDynamic, 1000 },
            { vk::DescriptorType::eInputAttachment, 1000 }
        };

        vk::DescriptorPoolCreateInfo poolInfo;
        poolInfo.flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet;
        poolInfo.maxSets = 1000;
        poolInfo.poolSizeCount = std::size(pool_sizes);
        poolInfo.pPoolSizes = pool_sizes;

        auto[result, pool] = device.createDescriptorPool(poolInfo);
        vk::resultCheck(result, "Failed to create descriptor pool");

        m_Handle = pool;
        

	}

	void VulkanDescriptorPool::Destroy()
	{
		m_Context->GetDevice()->GetDevice().destroyDescriptorPool(m_Handle);
	}
}
