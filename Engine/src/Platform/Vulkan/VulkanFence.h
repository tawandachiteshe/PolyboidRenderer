#pragma once
#include "Engine/Renderer/SyncObjects.h"
#include <vulkan/vulkan.hpp>

namespace Polyboid
{
	class VkRenderAPI;

	class VulkanFence : public Fence
	{

	private:

		vk::Fence m_Handle;
		const VkRenderAPI* m_Context = nullptr;

	public:
		explicit VulkanFence(const VkRenderAPI* context);


		virtual void Destroy();

		std::any GetHandle() override;
		~VulkanFence() override;
	};

	
}

