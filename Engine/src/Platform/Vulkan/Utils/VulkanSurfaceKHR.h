#pragma once
#include "Engine/Engine/Base.h"
#include <vulkan/vulkan_raii.hpp>


namespace std
{
	class any;
}



namespace Polyboid
{
	class VkInstance;

	class VulkanSurfaceKHR
	{
	private:

		vk::SurfaceKHR m_Surface;

	public:

		VulkanSurfaceKHR(const Ref<VkInstance>& instance, const std::any& nativWindow);
		operator vk::SurfaceKHR() const { return m_Surface; }
		operator vk::SurfaceKHR() { return m_Surface; }
		vk::SurfaceKHR GetSurface() { return m_Surface; }

	};

}
