#pragma once
#include "Engine/Renderer/Buffer.h"
#include <vma/vk_mem_alloc.h>

#include "Utils/VulkanAllocator.h"

namespace Polyboid
{
	class VkRenderAPI;

	class VulkanStagingBuffer : public StagingBuffer
	{
	private:
		vk::Buffer m_Buffer = nullptr;
		VmaAllocation m_Memory = nullptr;
		uint32_t m_Size = 0;
		const VkRenderAPI* m_Context = nullptr;
		VmaAllocationInfo m_AllocInfo{};
		VulkanAllocator m_Allocator;

	public:
		void Init(const VkRenderAPI* context, uint32_t size);
		void Recreate();
		VulkanStagingBuffer(const VkRenderAPI* context, const void* data, uint32_t size);
		VulkanStagingBuffer(const VkRenderAPI* context, uint32_t size);
		void SetData(const void* data) override;

		virtual void Destroy();
		uint32_t GetSizeInBytes() override;
		std::any GetHandle() override;
	};

}

