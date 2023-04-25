#pragma once
#include <vulkan/vulkan.hpp>

#include "Engine/Renderer/Swapchain.h"


namespace Polyboid
{
	class VulkanCommandList;
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
		static vk::SwapchainKHR s_Swapchain;
		vk::SurfaceCapabilitiesKHR m_Capabilities;
		std::vector<vk::SurfaceFormatKHR> m_Formats;
		std::vector<vk::PresentModeKHR> m_PresentModes;
		vk::SwapchainKHR m_Swapchain = nullptr;
		std::vector<vk::Image> m_SwapchainImages;
		VkRenderAPI* m_Context = nullptr;
		SwapchainSettings m_Settings;
		Ref<VulkanRenderPass> m_RenderPass = nullptr;
		uint32_t m_SwapchainCurrentImageIndex = 0;
		std::vector<Ref<VulkanTexture2D>> m_Textures;
		std::vector<Ref<VulkanFramebuffer>> m_Framebuffers;
		vk::Queue m_PresentQueue;


		bool m_Resize = false;

		void Init(VkRenderAPI* context, const SwapchainSettings& settings);

	public:

		void Init();


		operator vk::SwapchainKHR() const { return m_Swapchain; }
		VkSwapChain(VkRenderAPI* context, const SwapchainSettings& settings);
		static std::vector<Ref<VulkanTexture2D>> CreateSwapchainTextures(EngineGraphicsFormats imageFormat = EngineGraphicsFormats::BGRA8U);

		Ref<RenderPass> GetDefaultRenderPass() override;

		void Destroy(vk::Device device);

		void Resize(uint32_t width, uint32_t height) override;

		vk::SwapchainKHR GetVulkanSwapChain() { return m_Swapchain; }

		void Present(const Ref<Semaphore>& renderSemaphore) override;


		uint32_t GetImageIndex(const Ref<Semaphore>& imageSemaphore) override;
		virtual  Ref<VulkanFramebuffer> GetCurrentFramebuffer();
		void SetVsync(bool vsync) override;
		~VkSwapChain() override;
	};

}
