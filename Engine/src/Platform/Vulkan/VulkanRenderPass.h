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
		Ref <VulkanFramebuffer> m_Framebuffer;
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

		void Clear(TextureAttachmentSlot attachment, const ClearSettings& settings) override;
		void Clear(const ClearSettings& settings) override;
		ClearSettings GetClearSettings();

		RenderPassSettings& GetRenderPassSettings() override;

		~VulkanRenderPass() override;

		virtual  vk::RenderPassBeginInfo GetRenderBeginInfo(Ref <VulkanFramebuffer>& framebuffer);

		friend class VulkanFramebuffer;
		friend class VulkanCommandBuffer;
	};
}


