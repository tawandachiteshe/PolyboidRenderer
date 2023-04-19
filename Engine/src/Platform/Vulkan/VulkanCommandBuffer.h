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
		const VkRenderAPI* m_Context;
		const VulkanCommandList* m_CommandList;
		Ref<VulkanFence> m_Fence = nullptr;
		Ref<VkSwapChain> m_Swapchain = nullptr;

		//Used by swapchain
		Ref<VulkanSemaphore> m_ImageSemaphore = nullptr;
		Ref<VulkanSemaphore> m_RenderSemaphore = nullptr;
		uint32_t m_SwapchainCurrentImageIndex = 0;

	public:

		uint32_t GetImageIndex() { return m_SwapchainCurrentImageIndex; }
		void Destroy(const vk::Device& device);
		VulkanCommandBuffer(const VkRenderAPI* context, const VulkanCommandList* commands);
		void Begin() override;
		void End() override;
		void BeginRenderPass(const Ref<RenderPass>& renderPass) override;
		void EndRenderPass() override;
		void SubmitAndRender() override;
		void SubmitSwapchain(const Ref<Swapchain>& swapchain) override;
		void SubmitAndPresent() override;

		std::any GetHandle() override;

		~VulkanCommandBuffer() override;
	};

	
}

