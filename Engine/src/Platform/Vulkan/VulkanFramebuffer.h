#pragma once
#include "Engine/Renderer/Framebuffer.h"
#include <vulkan/vulkan.hpp>

#include "VulkanRenderPass.h"


namespace Polyboid
{
	class VulkanTexture2D;
	class VkRenderAPI;

	class VulkanFramebuffer : public Framebuffer
	{
	private:

		std::vector<vk::Framebuffer> m_FrameBuffers;

		FramebufferSettings m_Settings;
		const VkRenderAPI* m_Context{};
		VulkanRenderPass* m_RenderPassPtr = nullptr;
		std::unordered_map<TextureAttachmentSlot, Ref<VulkanTexture2D>> m_AttachmentTextures;
		std::vector<Ref<VulkanTexture2D>> m_Textures;

		void Init(const VkRenderAPI* context, const FramebufferSettings& settings);

	public:
		void Destroy(vk::Device device);
		VulkanFramebuffer(const VkRenderAPI* context, const FramebufferSettings& settings);
		VulkanFramebuffer(const VkRenderAPI* context, const FramebufferSettings& settings, RenderPass* renderPass);

		virtual vk::Framebuffer GetFramebufferHandle(uint32_t index = 0);

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

