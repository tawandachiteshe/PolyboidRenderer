#include "boidpch.h"
#include "VulkanRenderPass.h"
#include <vulkan/vulkan.hpp>

#include "VkRenderAPI.h"
#include "VkSwapChain.h"
#include "VulkanFramebuffer.h"
#include "Utils/Common.h"
#include "Utils/VulkanDevice.h"

namespace Polyboid
{
	void VulkanRenderPass::SetFramebuffer(const Ref<Framebuffer>& framebuffer)
	{
		auto vkFramebuffer = std::reinterpret_pointer_cast<VulkanFramebuffer>(framebuffer);
		m_Framebuffer = vkFramebuffer;
	}

	Ref<Framebuffer> VulkanRenderPass::GetFramebuffer()
	{
		return m_Framebuffer;
	}

	VulkanRenderPass::VulkanRenderPass(const VkRenderAPI* context, const RenderPassSettings& settings):
		m_Settings(settings), m_Context(context)
	{
		vk::Device device = (*context->GetDevice());

		// The initial layout for the color and depth attachments will be LAYOUT_UNDEFINED
// because at the start of the renderpass, we don't care about their contents.
// At the start of the subpass, the color attachment's layout will be transitioned
// to LAYOUT_COLOR_ATTACHMENT_OPTIMAL and the depth stencil attachment's layout
// will be transitioned to LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL.  At the end of
// the renderpass, the color attachment's layout will be transitioned to
// LAYOUT_PRESENT_SRC_KHR to be ready to present.  This is all done as part of
// the renderpass, no barriers are necessary.
		std::array<vk::AttachmentDescription, 1> const attachments = {
			vk::AttachmentDescription()
				.setFormat(vk::Format::eB8G8R8A8Unorm)
				.setSamples(vk::SampleCountFlagBits::e1)
				.setLoadOp(vk::AttachmentLoadOp::eClear)
				.setStoreOp(vk::AttachmentStoreOp::eStore)
				.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
				.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
				.setInitialLayout(vk::ImageLayout::eUndefined)
				.setFinalLayout(vk::ImageLayout::ePresentSrcKHR),
		};

		auto const color_reference = vk::AttachmentReference().setAttachment(0).setLayout(vk::ImageLayout::eColorAttachmentOptimal);

		auto const subpass = vk::SubpassDescription()
			.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
			.setColorAttachments(color_reference)
			.setPDepthStencilAttachment(nullptr);


		const auto render_pass_result = device.createRenderPass(
			vk::RenderPassCreateInfo().setAttachments(attachments).setSubpasses(subpass));
		vk::resultCheck(render_pass_result.result, "failed to create render pass");
		m_RenderPass = render_pass_result.value;

		m_ColorValue.float32[0] = m_ClearSettings.color.x;
		m_ColorValue.float32[1] = m_ClearSettings.color.y;
		m_ColorValue.float32[2] = m_ClearSettings.color.z;
		m_ColorValue.float32[3] = m_ClearSettings.color.w;

		m_ClearValues[0] = m_ColorValue;
	}

	void VulkanRenderPass::Destroy(vk::Device device)
	{
		device.destroyRenderPass(m_RenderPass);
	}


	void VulkanRenderPass::Clear(TextureAttachmentSlot attachment, const ClearSettings& settings)
	{
	}

	void VulkanRenderPass::Clear(const ClearSettings& settings)
	{
		m_ClearSettings = settings;

		m_ColorValue.float32[0] = settings.color.x;
		m_ColorValue.float32[1] = settings.color.y;
		m_ColorValue.float32[2] = settings.color.z;
		m_ColorValue.float32[3] = settings.color.w;

		m_ClearValues[0] = m_ColorValue;
	}

	ClearSettings VulkanRenderPass::GetClearSettings()
	{
		return m_ClearSettings;
	}

	RenderPassSettings& VulkanRenderPass::GetRenderPassSettings()
	{
		return m_Settings;
	}


	VulkanRenderPass::~VulkanRenderPass()
	{
	}

	vk::RenderPassBeginInfo VulkanRenderPass::GetRenderBeginInfo(Ref<VulkanFramebuffer>& framebuffer)
	{
		m_Framebuffer = framebuffer;
		m_RenderPassBeginInfo.sType = vk::StructureType::eRenderPassBeginInfo;
		m_RenderPassBeginInfo.renderPass = m_RenderPass;
		m_RenderPassBeginInfo.framebuffer = m_Framebuffer->GetFramebufferHandle();
		m_RenderPassBeginInfo.renderArea.offset = vk::Offset2D{0, 0};
		m_RenderPassBeginInfo.renderArea.extent = vk::Extent2D{m_Settings.Width, m_Settings.Height};
		m_RenderPassBeginInfo.clearValueCount = 1;
		m_RenderPassBeginInfo.pClearValues = m_ClearValues.data();

		vk::Framebuffer::NativeType fb = m_RenderPassBeginInfo.framebuffer;
		//spdlog::info("Framebuffer id {}", (uint64_t)fb);

		return m_RenderPassBeginInfo;
	}
}
