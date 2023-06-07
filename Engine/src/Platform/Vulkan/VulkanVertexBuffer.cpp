#include "boidpch.h"
#include "VulkanVertexBuffer.h"

#include <spdlog/spdlog.h>
#include <vulkan/vulkan_raii.hpp>


#include "VulkanCommandBufferSet.h"
#include "VulkanStagingBuffer.h"
#include "Engine/Renderer/CommandBufferSet.h"
#include "Engine/Renderer/GraphicsBackend.h"
#include "Engine/Renderer/RenderCommand.h"
#include "Utils/VulkanAllocatorInstance.h"
#include "Utils/VulkanDevice.h"

namespace Polyboid
{
	void VulkanVertexBuffer::Init(const VkRenderAPI* context, const void* data, uint32_t size)
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


		Ref<VulkanStagingBuffer> staging = CreateRef<VulkanStagingBuffer>(context, data, size);
		Ref<VulkanCommandBufferSet> cmdList = CommandBufferSet::Create({ 1 }).As<VulkanCommandBufferSet>();

		cmdList->GetCommandBufferAt(0)->Begin();
		const auto& cmdBuffer = cmdList->GetCommandBufferAt(0);
		cmdBuffer->CopyVertexBuffer(staging.As<StagingBuffer>(), this);
		cmdList->GetCommandBufferAt(0)->End();
		RenderCommand::GetGraphicsBackend()->SubmitOneTimeWork(cmdBuffer);

		staging->Destroy();
		cmdList->Destroy(device);
	}

	void VulkanVertexBuffer::Recreate()
	{
		Init(m_Context, m_Data, m_Size);
	}

	VulkanVertexBuffer::VulkanVertexBuffer(const VkRenderAPI* context, const void* data, uint32_t size) : m_Context(context), m_Data(data), m_Size(size)
	{
		Init(context, data, size);
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
