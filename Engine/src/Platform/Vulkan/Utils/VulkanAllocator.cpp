#include "boidpch.h"
#include "VulkanAllocator.h"

#include "VulkanDevice.h"
#include "VulkanPhysicalDevice.h"
#include "VkInstance.h"
#include "Platform/Vulkan/VkRenderAPI.h"
#include <vma/vk_mem_alloc.h>


namespace Polyboid
{
	VulkanAllocator::VulkanAllocator(const VkRenderAPI* context)
	{

		vk::Device device = *context->GetDevice();
		vk::PhysicalDevice physicalDevice = *context->GetPhysicalDevice();
		vk::Instance instance = *context->GetInstance();


		VmaAllocatorCreateInfo allocatorCreateInfo = {};
		allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_2;
		allocatorCreateInfo.physicalDevice = physicalDevice;
		allocatorCreateInfo.device = device;
		allocatorCreateInfo.instance = instance;

		vmaCreateAllocator(&allocatorCreateInfo, &m_Allocator);

		//vmaCreateImage(=)


		//vma::createAllocator(&info, &allocatorCreateInfo);

	}

	void VulkanAllocator::Destroy()
	{
		vmaDestroyAllocator(m_Allocator);
	}
}
