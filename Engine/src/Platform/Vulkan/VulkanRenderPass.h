#pragma once
#include "Engine/Renderer/RenderPass.h"
#include "vulkan/vulkan.hpp"

namespace Polyboid
{
	class VkRenderAPI;

	class VulkanRenderPass : public RenderPass
	{
	private:
		vk::RenderPass m_RenderPass = nullptr;
		ClearSettings m_ClearSettings;
		RenderPassSettings m_Settings;
		Ref <Framebuffer> m_Framebuffer;
		Ref<Swapchain> m_Swapchain;

	public:
		vk::RenderPass GetHandle() { return m_RenderPass; }
		void SetFramebuffer(const Ref<Framebuffer>& framebuffer) override;
		Ref<Framebuffer> GetFramebuffer() override;

		VulkanRenderPass(const VkRenderAPI* context, const RenderPassSettings& settings);
		void Destroy(vk::Device device);

		void AttachTexture(TextureAttachmentSlot attachment, Ref<Texture> texture) override;
		Ref<Texture> GetTexture(TextureAttachmentSlot attachment) override;
		void Clear(TextureAttachmentSlot attachment, const ClearSettings& settings) override;
		void Clear(const ClearSettings& settings) override;
		ClearSettings GetClearSettings();
		void GenerateMipMaps() override;
		~VulkanRenderPass() override;

		friend class VulkanFramebuffer;
		friend class VulkanCommandBuffer;
	};
}


