#include "boidpch.h"
#include "VulkanVertexBuffer.h"

#include <spdlog/spdlog.h>
#include <vulkan/vulkan.hpp>


#include "VulkanCommandList.h"
#include "VulkanStagingBuffer.h"
#include "Engine/Renderer/CommandList.h"
#include "Utils/VulkanAllocatorInstance.h"
#include "Utils/VulkanDevice.h"

namespace Polyboid
{
	VulkanVertexBuffer::VulkanVertexBuffer(const VkRenderAPI* context, const void* data, uint32_t size): m_Context(context)
	{
		auto device = context->GetDevice()->GetVulkanDevice();
		VmaAllocator allocator = *context->GetAllocator();
		

		vk::BufferCreateInfo createInfo;
		createInfo.usage = vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer;
		createInfo.size = size;
		m_Size = size;

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


		Ref<VulkanStagingBuffer> staging =  CreateRef<VulkanStagingBuffer>(context, data, size);
		Ref<VulkanCommandList> cmdList = CommandList::Create({ 1 }).As<VulkanCommandList>();

		cmdList->GetCommandBufferAt(0)->Begin();
		const auto& cmdBuffer = cmdList->GetCommandBufferAt(0);
		cmdBuffer->CopyVertexBuffer(staging.As<StagingBuffer>(), this);
		cmdList->GetCommandBufferAt(0)->End();
		RenderAPI::Get()->SubmitCommandBuffer(cmdBuffer);

		staging->Destroy();
		cmdList->Destroy(device);
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

	uint32_t VulkanVertexBuffer::GetSizeInBytes() const
	{
		return m_Size;
	}

	std::any VulkanVertexBuffer::GetHandle() const
	{
		return m_Handle;
	}

	void VulkanVertexBuffer::Destroy()
	{
		VmaAllocator allocator = *m_Context->GetAllocator();
		vmaDestroyBuffer(allocator, m_Handle, m_Allocation);
	}
}
