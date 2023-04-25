#pragma once
#include <vulkan/vulkan.hpp>

#include "VulkanCommandList.h"
#include "Engine/Renderer/CommandList.h"


namespace Polyboid
{
	class VkSwapChain;
	class VulkanSemaphore;
	class VulkanFence;
	class VulkanCommandList;
	class VkRenderAPI;

	class VulkanCommandBuffer : public CommandBuffer
	{
	private:
		vk::CommandBuffer m_CommandBuffer;
		const CommandList* m_CommandList = nullptr;
		const VkRenderAPI* m_Context;

	public:

		void Destroy(const vk::Device& device);
		VulkanCommandBuffer(const VkRenderAPI* context, const VulkanCommandList* commands);
		void Begin() override;
		void End() override;
		void BeginRenderPass(const Ref<RenderPass>& renderPass, const Ref<Framebuffer>& framebuffer) override;
		void EndRenderPass() override;

		void Reset() override;

		std::any GetHandle() override;

		~VulkanCommandBuffer() override;
	};

	
}

