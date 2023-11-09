#pragma once

#include <vulkan/vulkan.hpp>

namespace Polyboid
{
	class VulkanControlCenter
	{
	private:
		vk::Queue m_Queue;
		vk::Device m_Device;
	public:

		VulkanControlCenter();
		void Resize(uint32_t width, uint32_t height);

	};

}

