#pragma once
#include "Engine/Renderer/RenderPass.h"
#include "vulkan/vulkan_raii.hpp"

namespace Polyboid
{
	class FrameBufferSet;
	class VulkanFramebuffer;
	class VkRenderAPI;

	class VulkanRenderPass : public RenderPass
	{
	private:
		vk::RenderPass m_RenderPass = nullptr;
		ClearSettings m_ClearSettings;
		RenderPassSettings m_Settings;
		Ref <VulkanFramebuffer> m_CurrentFramebuffer = nullptr;
		Ref<FrameBufferSet> m_FrameBufferSet;
		Ref<Swapchain> m_Swapchain = nullptr;
		vk::RenderPassBeginInfo m_RenderPassBeginInfo{};
		vk::ClearColorValue m_ColorValue;
		vk::ClearDepthStencilValue m_DepthValue;
		std::array<vk::ClearValue, 2> m_ClearValues;
		const VkRenderAPI* m_Context = nullptr;
		uint32_t m_Width = 0, m_Height = 0;
		std::array<vk::AttachmentDescription, 2> m_Attachments;
		std::array<vk::SubpassDependency, 2> m_Dependencies;
		vk::SubpassDescription m_Subpass;
		std::vector<vk::AttachmentReference> m_AttachmentRefs;
		vk::RenderPassCreateInfo m_RenderpassCreateInfo;


	public:
		vk::RenderPass GetHandle() { return m_RenderPass; }
	
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
		

		Ref<Texture2D> GetColorTexture(const TextureAttachmentSlot& attachmentIndex, uint32_t frameIndex = 0) override;
		Ref<Texture2D> GetDepthTexture(uint32_t frameIndex = 0) override;

		~VulkanRenderPass() override;

		virtual  vk::RenderPassBeginInfo GetRenderBeginInfo();

		RenderResourceType GetRenderResourceType() override;

		friend class VulkanFramebuffer;
		friend class VulkanCommandBuffer;
	};
}


