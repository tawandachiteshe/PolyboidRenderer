#include "boidpch.h"
#include "VulkanSurfaceKHR.h"

#include <any>
#include <spdlog/spdlog.h>


#include "VkInstance.h"
#define GLFW_NATIVE_INCLUDE_NONE
#include "GLFW/glfw3native.h"
#include <GLFW/glfw3.h>


namespace Polyboid
{
	VulkanSurfaceKHR::VulkanSurfaceKHR(const Ref<VkInstance>& instance, const std::any& nativWindow )
	{
		vk::Instance vkInstance = (*instance);

		const auto window = std::any_cast<GLFWwindow*>(nativWindow);

		
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
