#include "boidpch.h"
#include "Buffers.h"

#include <spdlog/spdlog.h>

#include "VkRenderAPI.h"
#include "Utils/VulkanAllocator.h"
#include "Utils/VulkanDevice.h"


namespace Polyboid
{
	VulkanUniformBuffer::VulkanUniformBuffer(const VkRenderAPI* context, uint32_t size, uint32_t slot):
		m_Context(context), m_Size(size), m_Slot(slot)
	{
		auto device = context->GetDevice()->GetVulkanDevice();
		VmaAllocator allocator = *context->GetAllocator().get();


		vk::BufferCreateInfo createInfo;
		createInfo.usage = vk::BufferUsageFlagBits::eUniformBuffer;
		//vk::
		createInfo.size = size;
		m_Size = size;
		m_Slot = slot;

		vk::BufferCreateInfo::NativeType vkCreateInfo = createInfo;
		VmaAllocationCreateInfo vmaCreateInfo{};
		vmaCreateInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
		vmaCreateInfo.priority = 1.0f;


		VkBuffer buffer;

		VkResult result = vmaCreateBuffer(allocator, &vkCreateInfo, &vmaCreateInfo, &buffer, &m_Allocation, nullptr);

		m_UniformBuffer = buffer;

		if (result != VK_SUCCESS)
		{
			spdlog::info("Failed to create indexBuffer");
			__debugbreak();
		}


		m_DescBufferInfo.buffer = buffer;
		m_DescBufferInfo.offset = 0;
		m_DescBufferInfo.range = m_Size;
	
	}

	void VulkanUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
	}

	uint32_t VulkanUniformBuffer::GetBindingSlot()
	{
		return m_Slot;
	}

	uint32_t VulkanUniformBuffer::GetDataSize()
	{
		return m_Size;
	}

	void VulkanUniformBuffer::Destroy()
	{
		VmaAllocator allocator = *m_Context->GetAllocator().get();
		vmaDestroyBuffer(allocator, m_UniformBuffer, m_Allocation);
	}

	std::any VulkanUniformBuffer::GetHandle()
	{
		return m_UniformBuffer;
	}

	void VulkanShaderStorage::Destroy()
	{
		VmaAllocator allocator = *m_Context->GetAllocator().get();
		vmaDestroyBuffer(allocator, m_StorageBuffer, m_Allocation);
	}

	std::any VulkanShaderStorage::GetHandle()
	{
		return m_StorageBuffer;
	}

	vk::DescriptorBufferInfo VulkanShaderStorage::GetVulkanDescBuffer()
	{
		return m_DescBufferInfo;
	}

	vk::DescriptorBufferInfo VulkanUniformBuffer::GetVulkanDescBuffer()
	{
		return m_DescBufferInfo;
	}

	VulkanShaderStorage::VulkanShaderStorage(const VkRenderAPI* context, uint32_t size): m_Size(size),
		m_Context(context)
	{
		auto device = context->GetDevice()->GetVulkanDevice();
		VmaAllocator allocator = *context->GetAllocator().get();
		m_Allocator = allocator;


		vk::BufferCreateInfo createInfo;
		createInfo.usage = vk::BufferUsageFlagBits::eStorageBuffer;
		//vk::
		createInfo.size = size;
		m_Size = size;

		vk::BufferCreateInfo::NativeType vkCreateInfo = createInfo;
		VmaAllocationCreateInfo vmaCreateInfo{};
		vmaCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
		vmaCreateInfo.priority = 1.0f;


		VkBuffer buffer;

		VkResult result = vmaCreateBuffer(allocator, &vkCreateInfo, &vmaCreateInfo, &buffer, &m_Allocation, nullptr);
		m_StorageBuffer = buffer;

		if (result != VK_SUCCESS)
		{
			spdlog::info("Failed to create indexBuffer");
			__debugbreak();
		}

		m_DescBufferInfo.buffer = buffer;
		m_DescBufferInfo.offset = 0;
		m_DescBufferInfo.range = m_Size;
		
	}

	void VulkanShaderStorage::Bind(uint32_t slot) const
	{
	}

	void VulkanShaderStorage::Unbind() const
	{
	}

	void VulkanShaderStorage::SetData(const void* data, uint32_t sizeData, uint32_t offset)
	{

		void* mappedMem = nullptr;
		vmaMapMemory(m_Allocator, m_Allocation, &mappedMem);
		std::memcpy(mappedMem, data, sizeData);
		vmaUnmapMemory(m_Allocator, m_Allocation);

	}

	VulkanShaderStorage::~VulkanShaderStorage()
	{
	}
}
