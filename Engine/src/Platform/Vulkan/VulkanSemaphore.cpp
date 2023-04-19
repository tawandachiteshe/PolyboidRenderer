#include "boidpch.h"
#include "VulkanSemaphore.h"


#include "VkRenderAPI.h"
#include "Utils/VulkanDevice.h"

namespace Polyboid
{
	VulkanSemaphore::VulkanSemaphore(const VkRenderAPI* context): m_Context(context)
	{


		vk::Device device = (*context->GetDevice());
		vk::SemaphoreCreateInfo createInfo{};

		 auto [result, _semaphore] = device.createSemaphore(createInfo);
		 vk::resultCheck(result, "Failed to create semaphore");
		 m_Handle = _semaphore;

	}

	void VulkanSemaphore::Destroy()
	{
		vk::Device device = (*m_Context->GetDevice());
		device.destroySemaphore(m_Handle);
	}

	std::any VulkanSemaphore::GetHandle()
	{

		return m_Handle;
	}

	VulkanSemaphore::~VulkanSemaphore()
	{
	}
}
