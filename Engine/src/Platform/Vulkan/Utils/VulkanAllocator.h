#pragma once
#include <vma/vk_mem_alloc.h>

namespace Polyboid
{
	class VkRenderAPI;

	class VulkanAllocator
	{
	private:
		VmaAllocator m_Allocator;
	public:

		operator VmaAllocator() const { return m_Allocator; }
		
		VmaAllocator GetAllocator() { return m_Allocator; }
		explicit VulkanAllocator(const VkRenderAPI* context);
		void Destroy();

	};

}
