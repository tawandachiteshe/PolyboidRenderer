#include "boidpch.h"
#include "VulkanStagingBuffer.h"

#include <vulkan/vulkan_raii.hpp>
#include "VkRenderAPI.h"


namespace Polyboid
{
	void VulkanStagingBuffer::Init(const VkRenderAPI* context, uint32_t size)
	{
		auto [buffer, allocation] = m_Allocator.CreateBuffer(size, vk::BufferUsageFlagBits::eTransferSrc);
		m_Buffer = buffer;
	}

	void VulkanStagingBuffer::Recreate()
	{
		Destroy();
		Init(m_Context, m_Size);
	}

	VulkanStagingBuffer::VulkanStagingBuffer(const VkRenderAPI* context, const void* data, uint32_t size): m_Size(size),
	                                                                                                       m_Context(context), m_AllocInfo(), m_Allocator(std::format("Staging buffer {}", size))
	{

		auto [buffer, allocation] = m_Allocator.CreateBuffer(size, vk::BufferUsageFlagBits::eTransferSrc);
		m_Buffer = buffer;
		m_Allocator.MapData(data, size);
	}

	VulkanStagingBuffer::VulkanStagingBuffer(const VkRenderAPI* context, uint32_t size): m_Size(size), m_Context(context),
		m_Allocator(std::format("Staging buffer {}", size))
	{
		
		Init(m_Context, m_Size);
	}

	void VulkanStagingBuffer::SetData(const void* data)
	{

		m_Allocator.MapData(data, m_Size);

	}

	void VulkanStagingBuffer::SetData(const void* data, uint32_t size)
	{
		m_Allocator.MapData(data, size);
	}

	void VulkanStagingBuffer::Destroy()
	{
		m_Allocator.DestroyBuffer();
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
