#pragma once
#include <vma/vk_mem_alloc.h>

namespace Polyboid
{
	class VkRenderAPI;

	class VulkanAllocatorInstance
	{
	private:
		VmaAllocator m_Allocator{};
	public:

		operator VmaAllocator() const { return m_Allocator; }

		explicit VulkanAllocatorInstance(const VkRenderAPI* context);
		void Destroy();

	};

}
