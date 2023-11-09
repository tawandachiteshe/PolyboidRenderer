#include "boidpch.h"
#include "VulkanAllocator.h"

#include <spdlog/spdlog.h>

#include "Platform/Vulkan/VkRenderAPI.h"


namespace Polyboid
{
	VulkanAllocator::VulkanAllocator(const std::string& name): m_Name(name)
	{
		m_Allocator = VkRenderAPI::GetVulkanAllocator();
	}

	std::pair<vk::Buffer, VmaAllocation> VulkanAllocator::CreateBuffer(uint32_t size, vk::BufferUsageFlagBits usage)
	{
		vk::BufferCreateInfo createInfo;
		createInfo.usage = usage;
		createInfo.size = static_cast<vk::DeviceSize>(size);
		m_Size = size;

		VmaAllocationCreateInfo memCreateInfo{};
		memCreateInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT;
		memCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
		memCreateInfo.priority = 1.0f;

		vk::BufferCreateInfo::NativeType vkCreateInfo = createInfo;

		VkBuffer buffer;

		auto result = vmaCreateBuffer(m_Allocator, &vkCreateInfo, &memCreateInfo, &buffer, &m_Allocation, &m_AllocInfo);

		

		if (result != VK_SUCCESS)
		{
			spdlog::info("Failed to create indexBuffer");
			__debugbreak();
		}

		vmaSetAllocationName(m_Allocator, m_Allocation, m_Name.c_str());

		m_AllocAndBufferHandle.first = buffer;
		m_AllocAndBufferHandle.second = m_Allocation;

		return m_AllocAndBufferHandle;
	}

	void VulkanAllocator::DestroyBuffer()
	{
		vmaDestroyBuffer(m_Allocator, m_AllocAndBufferHandle.first, m_AllocAndBufferHandle.second);

		m_AllocAndBufferHandle.first = nullptr;
		m_AllocAndBufferHandle.second = nullptr;
	}

	void VulkanAllocator::MapData(const void* data, uint32_t size)
	{
		if (size)
		{
			m_Size = size;
		}
		std::memcpy(m_AllocInfo.pMappedData, data, m_Size);
	}
}
