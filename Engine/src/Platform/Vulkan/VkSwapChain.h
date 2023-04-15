#pragma once
#include <vulkan/vulkan.hpp>

#include "Engine/Renderer/Swapchain.h"


namespace Polyboid
{
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
		vk::SwapchainKHR m_Swapchain;
		std::vector<vk::Image> m_SwapchainImages;
		std::vector<vk::ImageView> m_SwapChainImageViews;
		std::vector<Ref<Texture>> m_Textures;
		VkRenderAPI* m_Context;
		SwapchainSettings m_Settings;
		Ref<VulkanRenderPass> m_RenderPass;

		bool m_Resize = false;

		vk::Semaphore m_ImageAvailableSemaphore;
		void Init(VkRenderAPI* context, const SwapchainSettings& settings);

	public:

		void Init();

		operator vk::SwapchainKHR() const { return m_Swapchain; }
		VkSwapChain(VkRenderAPI* context, const SwapchainSettings& settings);
		Ref<RenderPass> GetDefaultRenderPass() override;
		std::vector<Ref<Texture>>& GetTextures() { return m_Textures; }
	

		void BeginFrame();

		void Destroy(vk::Device device);

		void Resize(uint32_t width, uint32_t height) override;


		void SwapBuffers() override;
		void SetVsync(bool vsync) override;
		~VkSwapChain() override;
	};

}
