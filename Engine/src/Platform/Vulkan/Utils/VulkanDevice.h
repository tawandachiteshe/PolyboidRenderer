#pragma once
#include <vulkan/vulkan.hpp>

#include "Engine/Engine/Base.h"

namespace Polyboid
{
	class VulkanPhysicalDevice;

	class VulkanDevice
	{
	private:
		vk::Device m_Device;
		vk::Queue m_PresentQueue;
		vk::Queue m_GraphicsQueue;
		Ref<VulkanPhysicalDevice> m_PhysicalDevice;
	public:
		vk::Device& GetVulkanDevice() { return m_Device; }
		operator vk::Device() const { return m_Device; }
		operator vk::Device()  { return m_Device; }

		vk::Queue& GetGraphicsQueue() { return m_GraphicsQueue; }
		vk::Queue& GetPresentQueue() { return m_PresentQueue; }

		VulkanDevice(const Ref<VulkanPhysicalDevice>& device);

	};

}

