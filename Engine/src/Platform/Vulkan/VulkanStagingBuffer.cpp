#include "boidpch.h"
#include "VulkanStagingBuffer.h"

#include <spdlog/spdlog.h>
#include <vulkan/vulkan.hpp>

#include "VkRenderAPI.h"
#include "Utils/VulkanAllocator.h"

namespace Polyboid
{
	VulkanStagingBuffer::VulkanStagingBuffer(const VkRenderAPI* context, const void* data, uint32_t size): m_Size(size),
		m_Context(context)
	{
		VmaAllocator allocator = (*context->GetAllocator());


		vk::BufferCreateInfo createInfo;
		createInfo.usage = vk::BufferUsageFlagBits::eTransferSrc;
		createInfo.size = static_cast<vk::DeviceSize>(size);
		m_Size = size;

		VmaAllocationCreateInfo memCreateInfo{};
		memCreateInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT;
		memCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
		memCreateInfo.priority = 1.0f;

		vk::BufferCreateInfo::NativeType vkCreateInfo = createInfo;

		VkBuffer buffer;

		auto result = vmaCreateBuffer(allocator, &vkCreateInfo, &memCreateInfo, &buffer, &m_Memory, nullptr);

		m_Buffer = buffer;

		if (result != VK_SUCCESS)
		{
			spdlog::info("Failed to create indexBuffer");
			__debugbreak();
		}

		void* gpuData = nullptr;
		vmaMapMemory(allocator, m_Memory, &gpuData);
		std::memcpy(gpuData, data, size);
		vmaUnmapMemory(allocator, m_Memory);
	}

	void VulkanStagingBuffer::SetData(const void* data)
	{
		VmaAllocator allocator = (*m_Context->GetAllocator());

		void* gpuData = nullptr;
		vmaMapMemory(allocator, m_Memory, &gpuData);
		std::memcpy(gpuData, data, m_Size);
		vmaUnmapMemory(allocator, m_Memory);


	}

	void VulkanStagingBuffer::Destroy()
	{
		VmaAllocator allocator = *m_Context->GetAllocator().get();
		vmaDestroyBuffer(allocator, m_Buffer, m_Memory);
	}

	uint32_t VulkanStagingBuffer::GetSizeInBytes()
	{
		return m_Size;
	}

	std::any VulkanStagingBuffer::GetHandle()
	{
		return m_Buffer;
	}
}
