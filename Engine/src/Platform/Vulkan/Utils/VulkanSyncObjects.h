#pragma once
#include <vulkan/vulkan.hpp>

namespace Polyboid
{
	class VkRenderAPI;

	class VulkanSyncObjects
	{

	private:
		std::vector<vk::Semaphore> m_ImageAvaiSemaphores;
		std::vector<vk::Semaphore> m_RenderCompleteSemaphores;
		std::vector<vk::Fence> m_inFlightFences;
		uint32_t m_ObjectsCount = 0;

	public:

		std::vector<vk::Semaphore> GetImageSemaphores() { return m_ImageAvaiSemaphores; }
		std::vector<vk::Semaphore> GetRenderSemaphores() { return m_RenderCompleteSemaphores; }
		std::vector<vk::Fence> GetFences() { return m_inFlightFences; }
		

		void Destroy(vk::Device device);

		VulkanSyncObjects(const VkRenderAPI* context, uint32_t count);

	};

}

