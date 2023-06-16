#pragma once
#include "Engine/Renderer/RenderPass.h"
#include "vulkan/vulkan_raii.hpp"

namespace Polyboid
{
	class VulkanFramebuffer;
	class VkRenderAPI;

	class VulkanRenderPass : public RenderPass
	{
	private:
		vk::RenderPass m_RenderPass = nullptr;
		ClearSettings m_ClearSettings;
		RenderPassSettings m_Settings;
		Ref <VulkanFramebuffer> m_Framebuffer = nullptr;
		Ref<Swapchain> m_Swapchain = nullptr;
		vk::RenderPassBeginInfo m_RenderPassBeginInfo{};
		vk::ClearColorValue m_ColorValue;
		vk::ClearDepthStencilValue m_DepthValue;
		std::array<vk::ClearValue, 2> m_ClearValues;
		const VkRenderAPI* m_Context = nullptr;
		uint32_t m_Width = 0, m_Height = 0;

	public:
		vk::RenderPass GetHandle() { return m_RenderPass; }
		void SetFramebuffer(const Ref<Framebuffer>& framebuffer) override;
		Ref<Framebuffer> GetFramebuffer() override;

		VulkanRenderPass(const VkRenderAPI* context, uint32_t width, uint32_t height);
		VulkanRenderPass(const VkRenderAPI* context, const RenderPassSettings& settings);
		void InitSwapchain(const VkRenderAPI* context, uint32_t width, uint32_t height);
		void Init(const VkRenderAPI* context, const RenderPassSettings& settings);

		void Recreate() override;

		void Destroy() override;

		void Clear(TextureAttachmentSlot attachment, const ClearSettings& settings) override;
		void Clear(const ClearSettings& settings) override;
		ClearSettings GetClearSettings();
		void Resize(uint32_t width, uint32_t height) override;

		RenderPassSettings& GetRenderPassSettings() override;

		~VulkanRenderPass() override;

		virtual  vk::RenderPassBeginInfo GetRenderBeginInfo(Ref <VulkanFramebuffer>& framebuffer);

		friend class VulkanFramebuffer;
		friend class VulkanCommandBuffer;
	};
}


