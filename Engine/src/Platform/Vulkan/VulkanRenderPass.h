#pragma once
#include "Engine/Renderer/RenderPass.h"
#include "vulkan/vulkan.hpp"

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
		Ref <Framebuffer> m_Framebuffer;
		std::vector<Ref<VulkanFramebuffer>> m_Framebuffers;
		Ref<Swapchain> m_Swapchain;
		vk::RenderPassBeginInfo m_RenderPassBeginInfo{};
		vk::ClearColorValue m_ColorValue;
		vk::ClearDepthStencilValue m_DepthValue;
		std::array<vk::ClearValue, 1> m_ClearValues;
		const VkRenderAPI* m_Context = nullptr;

	public:
		vk::RenderPass GetHandle() { return m_RenderPass; }
		void SetFramebuffer(const Ref<Framebuffer>& framebuffer) override;
		Ref<Framebuffer> GetFramebuffer() override;

		VulkanRenderPass(const VkRenderAPI* context, const RenderPassSettings& settings);
		void Destroy(vk::Device device);
		std::vector<Ref<VulkanFramebuffer>>& GetFramebuffers() { return m_Framebuffers; }

		void AttachTexture(TextureAttachmentSlot attachment, Ref<Texture> texture) override;
		Ref<Texture> GetTexture(TextureAttachmentSlot attachment) override;
		void Clear(TextureAttachmentSlot attachment, const ClearSettings& settings) override;
		void Clear(const ClearSettings& settings) override;
		ClearSettings GetClearSettings();
		void GenerateMipMaps() override;
		~VulkanRenderPass() override;

		virtual  vk::RenderPassBeginInfo GetRenderBeginInfo();

		friend class VulkanFramebuffer;
		friend class VulkanCommandBuffer;
	};
}


