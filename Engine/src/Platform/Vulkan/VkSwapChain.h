#pragma once
#include <vulkan/vulkan.hpp>

#include "Engine/Renderer/Swapchain.h"


namespace Polyboid
{
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
		VkRenderAPI* m_Context;
		SwapchainSettings m_Settings;
		Ref<VulkanRenderPass> m_RenderPass = nullptr;


		bool m_Resize = false;

		vk::Semaphore m_ImageAvailableSemaphore;
		void Init(VkRenderAPI* context, const SwapchainSettings& settings);

	public:

		void Init();


		operator vk::SwapchainKHR() const { return m_Swapchain; }
		VkSwapChain(VkRenderAPI* context, const SwapchainSettings& settings);
		static std::vector<Ref<VulkanTexture2D>> CreateSwapchainTextures(EngineGraphicsFormats imageFormat = EngineGraphicsFormats::BGRA8U);

		Ref<RenderPass> GetDefaultRenderPass() override;

		void BeginFrame();

		void Destroy(vk::Device device);

		void Resize(uint32_t width, uint32_t height) override;

		void Invalidate() override;

		vk::SwapchainKHR GetSwapchain() { return m_Swapchain; }


		void SwapBuffers() override;
		void SetVsync(bool vsync) override;
		~VkSwapChain() override;
	};

}
