#pragma once
#include "Engine/Renderer/Framebuffer.h"
#include <vulkan/vulkan.hpp>

#include "VulkanRenderPass.h"


namespace Polyboid
{
	class VkRenderAPI;

	class VulkanFramebuffer : public Framebuffer
	{
	private:
		vk::Framebuffer m_Framebuffer;
		FramebufferSettings m_Settings;
		const VkRenderAPI* m_Context{};
		VulkanRenderPass* m_RenderPassPtr = nullptr;

		void Init(const VkRenderAPI* context, const FramebufferSettings& settings);

	public:
		void Destroy(vk::Device device);
		VulkanFramebuffer(const VkRenderAPI* context, const FramebufferSettings& settings);
		VulkanFramebuffer(const VkRenderAPI* context, const FramebufferSettings& settings, RenderPass* renderPass);

		void AttachRenderbuffer(const Ref<Renderbuffer>& renderbuffer, const TextureAttachmentSlot& slot) override;
		void AttachTexture(const Ref<Texture>& texture, const TextureAttachmentSlot& slot) override;
		void AttachTexture3D(const Ref<Texture3D>& texture, const TextureAttachmentSlot& slot) override;
		~VulkanFramebuffer() override;
		void Clear(TextureAttachmentSlot attachment, const ClearSettings& settings) override;
		void Clear(const ClearSettings& settings) override;

		void ReSize(uint32_t width, uint32_t height) override;

		friend class VulkanCommandBuffer;
		friend class VulkanRenderPass;
	};

}

