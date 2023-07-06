#pragma once
#include <vulkan/vulkan_raii.hpp>

#include "Engine/Renderer/Swapchain.h"


namespace Polyboid
{
	class Framebuffer;
	class VulkanCommandBufferSet;
	class VulkanCommandBuffer;
	class VulkanFence;
	class VulkanSemaphore;
	class VulkanTexture2D;
	class RenderPass;
	class VulkanRenderPass;
	class VulkanFramebuffer;
	class VkRenderAPI;

	class VkSwapChain : public Swapchain
	{
	private:
		vk::SurfaceCapabilitiesKHR m_Capabilities;
		std::vector<vk::SurfaceFormatKHR> m_Formats;
		std::vector<vk::PresentModeKHR> m_PresentModes;
		vk::SwapchainKHR m_Swapchain = nullptr;
		std::vector<vk::Image> m_SwapchainImages;
		VkRenderAPI* m_Context = nullptr;
		SwapchainSettings m_Settings;
		Ref<VulkanRenderPass> m_RenderPass = nullptr;
		uint32_t m_SwapchainCurrentImageIndex = 0;
		std::vector<Ref<VulkanTexture2D>> m_ColorTextures;
		std::vector<Ref<VulkanTexture2D>> m_DepthTextures;
		std::vector<Ref<VulkanFramebuffer>> m_Framebuffers;
		std::vector<Ref<Framebuffer>> m_FramebuffersRefs;
		vk::Queue m_PresentQueue;



		bool m_Resize = false;

		void Init(VkRenderAPI* context, const SwapchainSettings& settings);

	public:

		void Init();


		operator vk::SwapchainKHR() const { return m_Swapchain; }
		VkSwapChain(VkRenderAPI* context, const SwapchainSettings& settings);

		virtual vk::SwapchainKHR GetSwapChainHandle();

		Ref<RenderPass> GetRenderPass() override;

		void Destroy(vk::Device device);

		void Resize(uint32_t width, uint32_t height) override;

		vk::SwapchainKHR GetVulkanSwapChain() { return m_Swapchain; }

		std::any GetHandle() override;

		std::vector<Ref<Framebuffer>> GetFrameBuffers() override;
		Ref<Framebuffer> GetFrameBuffer(uint32_t index) override;

		Ref<Framebuffer> GetCurrentFrameBuffer() override;

		void SetVsync(bool vsync) override;
		~VkSwapChain() override;
	};

}
