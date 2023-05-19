#include "boidpch.h"
#include "VulkanIndexBuffer.h"

#include <spdlog/spdlog.h>

#include "VkRenderAPI.h"
#include "VulkanStagingBuffer.h"
#include "Engine/Renderer/CommandList.h"
#include "Engine/Renderer/RenderAPI.h"
#include "Engine/Renderer/CommandList/RenderCommand.h"
#include "VulkanCommandList.h"
#include "Utils/VulkanAllocator.h"
#include "Utils/VulkanDevice.h"


namespace Polyboid
{
	VulkanIndexBuffer::VulkanIndexBuffer(const VkRenderAPI* context, IndexDataType type, const std::variant<uint32_t*, uint16_t*>& data,
		uint32_t count): m_IndexType(type), m_Count(count)
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


		Ref<VulkanStagingBuffer> staging = std::make_shared<VulkanStagingBuffer>(context, IndicesData, static_cast<uint32_t>(createInfo.size));
		Ref<VulkanCommandList> cmdList = std::reinterpret_pointer_cast<VulkanCommandList>(CommandList::Create({1}));

		RenderCommand::BeginCommands({ cmdList });
		const auto& cmdBuffer = cmdList->GetCommandBufferAt(0);
		cmdBuffer->CopyIndexBuffer(staging, this);
		RenderCommand::EndCommands({ cmdList });
		RenderCommand::SubmitCommandBuffer({ cmdList });

		staging->Destroy();
		cmdList->Destroy(device);

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
		VmaAllocator allocator = *m_Context->GetAllocator().get();
		vmaDestroyBuffer(allocator, m_Handle, m_Allocation);

	}
}
