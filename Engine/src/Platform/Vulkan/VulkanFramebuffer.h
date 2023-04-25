﻿#pragma once
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

		vk::Framebuffer m_FrameBuffer;

		FramebufferSettings m_Settings;
		const VkRenderAPI* m_Context{};
		const VulkanRenderPass* m_RenderPassPtr = nullptr;
		std::unordered_map<TextureAttachmentSlot, Ref<VulkanTexture2D>> m_AttachmentTextures;
		std::vector<Ref<VulkanTexture2D>> m_Textures;

		void Init(const VkRenderAPI* context, const FramebufferSettings& settings);

	public:
		void Destroy(vk::Device device);
		VulkanFramebuffer(const VkRenderAPI* context, const FramebufferSettings& settings);
		VulkanFramebuffer(const VkRenderAPI* context, const FramebufferSettings& settings, const VulkanRenderPass* renderPass, std::vector<Ref<VulkanTexture2D>>& textures);


		virtual vk::Framebuffer GetFramebufferHandle();


		~VulkanFramebuffer() override;
		void Clear(TextureAttachmentSlot attachment, const ClearSettings& settings) override;
		void Clear(const ClearSettings& settings) override;

		void ReSize(uint32_t width, uint32_t height) override;

		friend class VulkanCommandBuffer;
		friend class VulkanRenderPass;
	};

}

