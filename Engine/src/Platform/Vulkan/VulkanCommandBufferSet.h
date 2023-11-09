#pragma once
#include "Engine/Renderer/CommandBufferSet.h"
#include "vulkan/vulkan.hpp"
#include <vector>

namespace Polyboid
{
	class VulkanCommandBuffer;
	class VkRenderAPI;

	class VulkanCommandBufferSet : public CommandBufferSet
	{
	private:

		const VkRenderAPI* m_Context;
		vk::CommandPool m_CommandList;
		std::vector<Ref<VulkanCommandBuffer>> m_CommandBuffers;
		vk::Queue m_PresentQueue;
		vk::Queue m_GraphicsQueue;
		bool m_CanPresent = false;
		CommandListSettings m_Settings{};

	public:


		VulkanCommandBufferSet(const VkRenderAPI* context, const CommandListSettings& settings);
		void Init(const VkRenderAPI* context, const CommandListSettings& settings);
		void Recreate();
		CommandListSettings& GetSettings() override;
		Ref<CommandBuffer> GetCommandBufferAt(uint32_t index) override;
		~VulkanCommandBufferSet() override;
		RenderResourceType GetRenderResourceType() override;

		void Destroy() override;

		friend class VulkanCommandBuffer;
	};
}


