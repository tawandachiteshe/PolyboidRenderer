#pragma once
#include "Engine/Renderer/Framebuffer.h"
#include <vulkan/vulkan_raii.hpp>

#include "VulkanRenderPass.h"


namespace Polyboid
{
	class VulkanTexture2D;
	class VkRenderAPI;

	class VulkanFramebuffer : public Framebuffer
	{
	private:

		vk::Framebuffer m_FrameBuffer;

		FramebufferSettings m_Settings;
		const VkRenderAPI* m_Context{};
		const VulkanRenderPass* m_RenderPassPtr = nullptr;
		Ref<VulkanRenderPass> m_RenderPass;
		std::unordered_map<TextureAttachmentSlot, Ref<VulkanTexture2D>> m_AttachmentTextures;
		std::vector<Ref<VulkanTexture2D>> m_Textures;

		void SwapChainInit(const VkRenderAPI* context, const FramebufferSettings& settings);
		void Init(const VkRenderAPI* context, const Ref<VulkanRenderPass>& renderPass);

	public:
		void Destroy() override;

		void SwapchainRecreate();
		void Recreate() override;

		
		VulkanFramebuffer(const VkRenderAPI* context, const Ref<VulkanRenderPass>& renderPass);
		VulkanFramebuffer(const VkRenderAPI* context, const FramebufferSettings& settings, const VulkanRenderPass* renderPass, std::vector<Ref<VulkanTexture2D>>& textures);


		virtual vk::Framebuffer GetFramebufferHandle();


		~VulkanFramebuffer() override;
		Ref<Texture2D> GetColorAttachment(TextureAttachmentSlot attachment) override;
		Ref<Texture2D> GetDepthAttachment() override;

		void ReSize(uint32_t width, uint32_t height) override;
		std::any GetHandle() override;

		friend class VulkanCommandBuffer;
		friend class VulkanRenderPass;
	};

}

