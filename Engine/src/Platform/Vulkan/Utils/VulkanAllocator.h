#pragma once
#include "Platform/Vulkan/Buffers.h"

namespace Polyboid
{


	class VulkanAllocator
	{
	private:
		std::string m_Name;
		VmaAllocator m_Allocator;
		VmaAllocation m_Allocation;
		VmaAllocationInfo m_AllocInfo;
		std::pair<vk::Buffer, VmaAllocation> m_AllocAndBufferHandle;
		uint32_t m_Size;
	public:
		explicit VulkanAllocator(const std::string& name);
		std::pair<vk::Buffer, VmaAllocation> CreateBuffer(uint32_t size, vk::BufferUsageFlagBits usage);
		void DestroyBuffer();
		void MapData(const void* data, uint32_t size = 0);
		
	};

}

