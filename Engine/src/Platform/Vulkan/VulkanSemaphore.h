#pragma once
#include "Engine/Renderer/SyncObjects.h"
#include "vulkan/vulkan.hpp"

namespace Polyboid
{
	class VkRenderAPI;

	class VulkanSemaphore : public Semaphore
	{
	private:
		vk::Semaphore m_Handle;
		const VkRenderAPI* m_Context = nullptr;

	public:
		explicit VulkanSemaphore(const VkRenderAPI* context);

		void Destroy();

		std::any GetHandle() override;
		~VulkanSemaphore() override;
	};

}

