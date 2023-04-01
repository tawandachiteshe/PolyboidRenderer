#include "boidpch.h"
#include "VulkanSyncObjects.h"

#include <spdlog/spdlog.h>

#include "VulkanDevice.h"
#include "Platform/Vulkan/VkRenderAPI.h"

namespace Polyboid
{
	void VulkanSyncObjects::Destroy(vk::Device device)
	{

		for (auto& semaphore : m_ImageAvaiSemaphores)
		{
			device.destroySemaphore(semaphore);
		}

		for (auto& semaphore : m_RenderCompleteSemaphores)
		{
			device.destroySemaphore(semaphore);
		}


		for (auto& fence : m_inFlightFences)
		{
			device.destroyFence(fence);
		}
	
	}

	VulkanSyncObjects::VulkanSyncObjects(const VkRenderAPI* context, uint32_t count)
	{

		m_ImageAvaiSemaphores.resize(count);
		m_RenderCompleteSemaphores.resize(count);
		m_inFlightFences.resize(count);
		vk::Device device = (*context->GetDevice());


		vk::SemaphoreCreateInfo createInfo{};

		for (auto& semaphore : m_ImageAvaiSemaphores)
		{
			auto [result, _semaphore] = device.createSemaphore(createInfo);
			vk::resultCheck(result, "Failed to create semaphore");
			semaphore = _semaphore;
		}

		for (auto& semaphore : m_RenderCompleteSemaphores)
		{
			auto [result, _semaphore] = device.createSemaphore(createInfo);
			vk::resultCheck(result, "Failed to create semaphore");
			semaphore = _semaphore;
		}


		vk::FenceCreateInfo fenceCreateInfo{};
		fenceCreateInfo.flags = vk::FenceCreateFlagBits::eSignaled;

		for (auto& fence : m_inFlightFences)
		{
			auto [result, _fence] = device.createFence(fenceCreateInfo);

			vk::resultCheck(result, "Failed to create fence");

			fence = _fence;
		}
		

		spdlog::info("Created Sync objects successfully");

	}
}
