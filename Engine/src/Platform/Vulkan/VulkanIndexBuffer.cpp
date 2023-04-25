#include "boidpch.h"
#include "VulkanIndexBuffer.h"

#include <spdlog/spdlog.h>

#include "VkRenderAPI.h"
#include "Engine/Renderer/RenderAPI.h"
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
		createInfo.usage = vk::BufferUsageFlagBits::eIndexBuffer;

		switch (type)
		{
		case IndexDataType::UnsignedShort: createInfo.size = static_cast<vk::DeviceSize>(count * sizeof(uint16_t));  break;
		case IndexDataType::UnsignedInt: createInfo.size = static_cast<vk::DeviceSize>(count * sizeof(uint32_t)); break;
		}

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

		void* mappedData = nullptr;
		vmaMapMemory(allocator, m_Allocation, &mappedData);

		std::memcpy(mappedData, std::get<uint32_t*>(data), createInfo.size);

		vmaUnmapMemory(allocator, m_Allocation);

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

	void VulkanIndexBuffer::Destroy()
	{
		VmaAllocator allocator = *m_Context->GetAllocator().get();
		vmaDestroyBuffer(allocator, m_Handle, m_Allocation);

	}
}
