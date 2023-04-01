#pragma once
#include <vulkan/vulkan.hpp>

namespace Polyboid
{
	class VkRenderAPI;

	class VulkanDescriptorPool
	{

	private:
		vk::DescriptorPool m_Handle;
		const VkRenderAPI* m_Context = nullptr;

	public:

		vk::DescriptorPool GetPool() { return m_Handle; }
		VulkanDescriptorPool(const VkRenderAPI* m_Context);
		void Destroy();



	};

}

