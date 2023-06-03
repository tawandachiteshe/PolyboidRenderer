#include "boidpch.h"
#include "VulkanIndexBuffer.h"

#include <spdlog/spdlog.h>

#include "VkRenderAPI.h"
#include "VulkanStagingBuffer.h"
#include "Engine/Renderer/CommandBufferSet.h"
#include "Engine/Renderer/RenderAPI.h"
#include "VulkanCommandBufferSet.h"
#include "Engine/Renderer/GraphicsBackend.h"
#include "Engine/Renderer/Renderer.h"
#include "Utils/VulkanAllocatorInstance.h"
#include "Utils/VulkanDevice.h"


namespace Polyboid
{
	VulkanIndexBuffer::VulkanIndexBuffer(const VkRenderAPI* context, IndexDataType type, const std::variant<uint32_t*, uint16_t*>& data,
		uint32_t count): m_IndexType(type), m_Count(count), m_Context(context), m_Data(data)
	{
		Init(context, type, data, count);
	}

	void VulkanIndexBuffer::Init(const VkRenderAPI* context, IndexDataType type,
		const std::variant<uint32_t*, uint16_t*>& data, uint32_t count)
	{
		auto device = context->GetDevice()->GetVulkanDevice();
		VmaAllocator allocator = *context->GetAllocator();


		vk::BufferCreateInfo createInfo;
		createInfo.usage = vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer;

		void* IndicesData = nullptr;

		switch (type)
		{
		case IndexDataType::UnsignedShort: IndicesData = std::get<uint16_t*>(data); createInfo.size = static_cast<vk::DeviceSize>(count * sizeof(uint16_t));  break;
		case IndexDataType::UnsignedInt:  IndicesData = std::get<uint32_t*>(data); createInfo.size = static_cast<vk::DeviceSize>(count * sizeof(uint32_t)); break;
		}

		m_Size = static_cast<uint32_t>(createInfo.size);

		vk::BufferCreateInfo::NativeType vkCreateInfo = createInfo;
		VmaAllocationCreateInfo vmaCreateInfo{};
		vmaCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
		vmaCreateInfo.priority = 1.0f;


		VkBuffer buffer;

		auto result = vmaCreateBuffer(allocator, &vkCreateInfo, &vmaCreateInfo, &buffer, &m_Allocation, nullptr);

		if (result != VK_SUCCESS)
		{
			spdlog::info("Failed to create indexBuffer");
			__debugbreak();
		}
		m_Handle = buffer;


		Ref<VulkanStagingBuffer> staging = CreateRef<VulkanStagingBuffer>(context, IndicesData, static_cast<uint32_t>(createInfo.size));
		Ref<VulkanCommandBufferSet> cmdList = CommandBufferSet::Create({ 1 }).As<VulkanCommandBufferSet>();

		const auto& cmdBuffer = cmdList->GetCommandBufferAt(0);
		cmdBuffer->Begin();
		cmdBuffer->CopyIndexBuffer(staging.As<StagingBuffer>(), this);
		cmdBuffer->End();
		Renderer::GetGraphicsBackend()->SubmitOneTimeWork(cmdBuffer);

		staging->Destroy();
		cmdList->Destroy(device);
	}

	void VulkanIndexBuffer::Recreate()
	{
		Destroy();
		Init(m_Context, m_IndexType, m_Data, m_Count);
	}


	VulkanIndexBuffer::~VulkanIndexBuffer()
	{
	}

	void VulkanIndexBuffer::Bind() const
	{
	}

	void VulkanIndexBuffer::Unbind() const
	{
	}

	IndexDataType VulkanIndexBuffer::GetIndexDataType()
	{
		return IndexDataType::UnsignedInt;
	}

	uint32_t VulkanIndexBuffer::GetCount() const
	{
		return 0;
	}

	uint32_t VulkanIndexBuffer::GetSizeInBytes() const
	{
		return m_Size;
	}

	std::any VulkanIndexBuffer::GetHandle() const
	{
		return m_Handle;
	}

	void VulkanIndexBuffer::Destroy()
	{
		VmaAllocator allocator = VkRenderAPI::GetVulkanAllocator();
		vmaDestroyBuffer(allocator, m_Handle, m_Allocation);

	}
}
