#pragma once
#include <vulkan/vulkan.hpp>

#include "VulkanCommandList.h"
#include "Engine/Renderer/CommandList.h"


namespace Polyboid
{
	class VulkanCommandList;
	class VkRenderAPI;

	class VulkanCommandBuffer : public CommandBuffer
	{
	private:
		std::vector<vk::CommandBuffer> m_CommandBuffer;
		const VkRenderAPI* m_Context;
		const VulkanCommandList* m_CommandList;
	public:

		std::vector<vk::CommandBuffer>& GetCommandBuffers() { return m_CommandBuffer; }
		vk::CommandPool GetCommandPool() { return m_CommandList->m_CommandList; }
		VulkanCommandBuffer(const VkRenderAPI* context, const VulkanCommandList* commands);
		void Begin() override;
		void End() override;
		void BeginRenderPass(const Ref<RenderPass>& renderPass) override;
		void EndRenderPass() override;
		void WaitAndRender() override;

		~VulkanCommandBuffer() override;
	};

	
}

