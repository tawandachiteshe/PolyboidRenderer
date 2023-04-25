#include "boidpch.h"
#include "VulkanVertexBuffer.h"

#include <spdlog/spdlog.h>
#include <vulkan/vulkan.hpp>


#include "Utils/VulkanAllocator.h"
#include "Utils/VulkanDevice.h"

namespace Polyboid
{
	VulkanVertexBuffer::VulkanVertexBuffer(const VkRenderAPI* context, const void* data, uint32_t size): m_Context(context)
	{
		auto device = context->GetDevice()->GetVulkanDevice();
		VmaAllocator allocator = *context->GetAllocator().get();
		

		vk::BufferCreateInfo createInfo;
		createInfo.usage = vk::BufferUsageFlagBits::eVertexBuffer;
		//vk::
		createInfo.size = size;

		vk::BufferCreateInfo::NativeType vkCreateInfo = createInfo;
		VmaAllocationCreateInfo vmaCreateInfo{};
		vmaCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
		vmaCreateInfo.priority = 1.0f;


		VkBuffer buffer;

		VkResult result = vmaCreateBuffer(allocator, &vkCreateInfo, &vmaCreateInfo, &buffer, &m_Allocation, nullptr);

		m_Handle = buffer;

		if (result != VK_SUCCESS)
		{
			spdlog::info("Failed to create indexBuffer");
			__debugbreak();
		}

		void* mappedData = nullptr;
		result = vmaMapMemory(allocator, m_Allocation, &mappedData);

		if (result != VK_SUCCESS)
		{
			spdlog::info("Failed to map indexBuffer");
			__debugbreak();
		}


		std::memcpy(mappedData, data, createInfo.size);

		vmaUnmapMemory(allocator, m_Allocation);


	}

	VulkanVertexBuffer::VulkanVertexBuffer(const VkRenderAPI* context, uint32_t size)
	{
	}

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
	}

	void VulkanVertexBuffer::Bind() const
	{
	}

	void VulkanVertexBuffer::Unbind() const
	{
	}

	void VulkanVertexBuffer::SetData(const void* data, uint32_t size)
	{
	}

	const BufferLayout& VulkanVertexBuffer::GetLayout() const
	{
		return m_Layout;
	}

	void VulkanVertexBuffer::SetLayout(const BufferLayout& layout)
	{
		m_Layout = layout;
	}

	void VulkanVertexBuffer::Destroy()
	{
		VmaAllocator allocator = *m_Context->GetAllocator().get();
		vmaDestroyBuffer(allocator, m_Handle, m_Allocation);
	}
}
