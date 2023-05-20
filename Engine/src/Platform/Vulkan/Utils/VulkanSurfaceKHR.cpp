#include "boidpch.h"
#include "VulkanSurfaceKHR.h"

#include <any>
#include <spdlog/spdlog.h>


#include "VkInstance.h"
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.hpp>


namespace Polyboid
{
	VulkanSurfaceKHR::VulkanSurfaceKHR(const Ref<VkInstance>& instance, const std::any& nativWindow )
	{
		vk::Instance vkInstance = (*instance);

		const auto window = std::any_cast<GLFWwindow*>(nativWindow);

		//auto const createInfo = vk::Win32SurfaceCreateInfoKHR().setHinstance(nullptr).setHwnd(window);
		
		
		VkSurfaceKHR surface;
		if(glfwCreateWindowSurface(vkInstance, window, nullptr, &surface) != VK_SUCCESS)
		{
			spdlog::error("Failed to create surface");
			__debugbreak();
		}

		m_Surface = surface;

		spdlog::info("Surface created successfully");
		
	}
}
