#pragma once
#include "Engine/Engine/Base.h"
#include <vulkan/vulkan_raii.hpp>

namespace Polyboid
{
	class VulkanSurfaceKHR;
	class VkInstance;

	struct QueueFamilyIndices {
		std::optional<uint32_t> GraphicsFamily;
		std::optional<uint32_t> PresentFamily;
		

		bool IsComplete() const
		{
			return  GraphicsFamily.has_value() && PresentFamily.has_value();
		}
	};

	class VulkanPhysicalDevice
	{
		private:
			vk::PhysicalDevice m_Device;
			QueueFamilyIndices m_Indices;
	public:

		operator vk::PhysicalDevice() const { return m_Device; }
		operator vk::PhysicalDevice() { return m_Device; }

		VulkanPhysicalDevice(const Ref<VkInstance>& instance, const Ref<VulkanSurfaceKHR>& surface);
		vk::PhysicalDevice& GetPhysicalDevice() { return m_Device; }
		QueueFamilyIndices& GetFamilyIndices() { return m_Indices; }
	};

}

