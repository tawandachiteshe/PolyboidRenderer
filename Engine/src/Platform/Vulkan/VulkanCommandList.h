#pragma once
#include "Engine/Renderer/CommandList.h"
#include "vulkan/vulkan.hpp"

namespace Polyboid
{
	class VulkanCommandBuffer;
	class VkRenderAPI;

	class VulkanCommandList : public CommandList
	{
	private:

		const VkRenderAPI* m_Context;
		vk::CommandPool m_CommandList;
		std::vector<Ref<VulkanCommandBuffer>> m_CommandBuffers;
		vk::Queue m_PresentQueue;
		vk::Queue m_GraphicsQueue;
		bool m_CanPresent = false;

	public:

		void Destroy(vk::Device device);

		VulkanCommandList(const VkRenderAPI* context, bool canPresent);
		void CreateCommandBuffers(uint32_t count) override;
		Ref<CommandBuffer> GetCommandBufferAt(uint32_t index) override;
		bool CanPresent() override;

		~VulkanCommandList() override;

		friend class VulkanCommandBuffer;
	};
}


