#include "boidpch.h"
#include "VulkanRenderPass.h"
#include <vulkan/vulkan_raii.hpp>

#include "VkRenderAPI.h"
#include "VkSwapChain.h"
#include "VulkanFramebuffer.h"
#include "Engine/Renderer/BufferSet.h"
#include "Engine/Renderer/RenderCommand.h"
#include "Utils/Common.h"
#include "Utils/VulkanDevice.h"

namespace Polyboid
{

	VulkanRenderPass::VulkanRenderPass(const VkRenderAPI* context, uint32_t width, uint32_t height): m_Context(context), m_Width(width), m_Height(height)
	{
		InitSwapchain(context, width, height);
	}

	VulkanRenderPass::VulkanRenderPass(const VkRenderAPI* context, const RenderPassSettings& settings):
		m_Settings(settings), m_Context(context)
	{
		Init(context, m_Settings);
	}

	void VulkanRenderPass::InitSwapchain(const VkRenderAPI* context, uint32_t width, uint32_t height)
	{
		vk::Device device = (*context->GetDevice());


		m_Settings.Width = width;
		m_Settings.Height = height;
		m_Settings.type = RenderPassType::Present;
		m_Settings.TextureAttachments = { { TextureAttachmentSlot::Color0, EngineGraphicsFormats::BGRA8U } };
		m_Settings.debugName = "Swapchain Renderpass";


		std::array<vk::SubpassDependency, 2> dependencies = {
			vk::SubpassDependency()  // Image layout transition
			.setSrcSubpass(VK_SUBPASS_EXTERNAL)
			.setDstSubpass(0)
			.setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
			.setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
			.setSrcAccessMask(vk::AccessFlagBits())
			.setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eColorAttachmentRead)
			.setDependencyFlags(vk::DependencyFlagBits::eByRegion),
			vk::SubpassDependency()  // Image layout transition
			.setSrcSubpass(VK_SUBPASS_EXTERNAL)
			.setDstSubpass(0)
			.setSrcStageMask(vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests)
			.setDstStageMask(vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests)
			.setSrcAccessMask(vk::AccessFlagBits())
			.setDstAccessMask(vk::AccessFlagBits::eDepthStencilAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentRead)
			.setDependencyFlags(vk::DependencyFlagBits::eByRegion)
		};



		std::array<vk::AttachmentDescription, 2> const attachments = {
			vk::AttachmentDescription()
				.setFormat(vk::Format::eB8G8R8A8Unorm)
				.setSamples(vk::SampleCountFlagBits::e1)
				.setLoadOp(vk::AttachmentLoadOp::eClear)
				.setStoreOp(vk::AttachmentStoreOp::eStore)
				.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
				.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
				.setInitialLayout(vk::ImageLayout::eUndefined)
				.setFinalLayout(vk::ImageLayout::ePresentSrcKHR),
			vk::AttachmentDescription()
				.setFormat(vk::Format::eD32SfloatS8Uint)
				.setSamples(vk::SampleCountFlagBits::e1)
				.setLoadOp(vk::AttachmentLoadOp::eClear)
				.setStoreOp(vk::AttachmentStoreOp::eDontCare)
				.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
				.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
				.setInitialLayout(vk::ImageLayout::eUndefined)
				.setFinalLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal)
		};

		auto const color_reference = vk::AttachmentReference().setAttachment(0).setLayout(vk::ImageLayout::eColorAttachmentOptimal);
		auto const depth_reference = vk::AttachmentReference().setAttachment(1).setLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

		auto const subpass = vk::SubpassDescription()
			.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
			.setColorAttachments(color_reference)
			.setPDepthStencilAttachment(&depth_reference);


		const auto render_pass_result = device.createRenderPass(
			vk::RenderPassCreateInfo().setAttachments(attachments).setSubpasses(subpass).setDependencies(dependencies));
		vk::resultCheck(render_pass_result.result, "failed to create render pass");
		m_RenderPass = render_pass_result.value;

		m_ColorValue.float32[0] = m_ClearSettings.color.x;
		m_ColorValue.float32[1] = m_ClearSettings.color.y;
		m_ColorValue.float32[2] = m_ClearSettings.color.z;
		m_ColorValue.float32[3] = m_ClearSettings.color.w;

		m_DepthValue.depth = 1.0f;
		m_DepthValue.stencil = 0u;

		m_ClearValues[0] = m_ColorValue;
		m_ClearValues[1] = m_DepthValue;
	}

	void VulkanRenderPass::Init(const VkRenderAPI* context, const RenderPassSettings& settings)
	{
		vk::Device device = (*context->GetDevice());

		std::array<vk::SubpassDependency, 2> dependencies = {
		vk::SubpassDependency()  // Image layout transition
		.setSrcSubpass(VK_SUBPASS_EXTERNAL)
		.setDstSubpass(0)
		.setSrcStageMask(vk::PipelineStageFlagBits::eFragmentShader)
		.setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
		.setSrcAccessMask(vk::AccessFlagBits::eShaderRead)
		.setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eColorAttachmentRead)
		.setDependencyFlags(vk::DependencyFlagBits::eByRegion),
		vk::SubpassDependency()  // Image layout transition
		.setSrcSubpass(0)
		.setDstSubpass(VK_SUBPASS_EXTERNAL)
		.setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
		.setDstStageMask(vk::PipelineStageFlagBits::eFragmentShader)
		.setSrcAccessMask(vk::AccessFlagBits::eColorAttachmentWrite)
		.setDstAccessMask(vk::AccessFlagBits::eShaderRead)
		.setDependencyFlags(vk::DependencyFlagBits::eByRegion)
		};



		std::array<vk::AttachmentDescription, 2> const attachments = {
			vk::AttachmentDescription()
				.setFormat(vk::Format::eR8G8B8A8Unorm)
				.setSamples(vk::SampleCountFlagBits::e1)
				.setLoadOp(vk::AttachmentLoadOp::eClear)
				.setStoreOp(vk::AttachmentStoreOp::eStore)
				.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
				.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
				.setInitialLayout(vk::ImageLayout::eUndefined)
				.setFinalLayout(vk::ImageLayout::eShaderReadOnlyOptimal),
			vk::AttachmentDescription()
				.setFormat(vk::Format::eD32SfloatS8Uint)
				.setSamples(vk::SampleCountFlagBits::e1)
				.setLoadOp(vk::AttachmentLoadOp::eClear)
				.setStoreOp(vk::AttachmentStoreOp::eDontCare)
				.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
				.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
				.setInitialLayout(vk::ImageLayout::eUndefined)
				.setFinalLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal)
		};

		auto const color_reference = vk::AttachmentReference().setAttachment(0).setLayout(vk::ImageLayout::eColorAttachmentOptimal);
		auto const depth_reference = vk::AttachmentReference().setAttachment(1).setLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

		auto const subpass = vk::SubpassDescription()
			.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
			.setColorAttachments(color_reference)
			.setPDepthStencilAttachment(&depth_reference);


		const auto render_pass_result = device.createRenderPass(
			vk::RenderPassCreateInfo().setAttachments(attachments).setSubpasses(subpass).setDependencies(dependencies));
		vk::resultCheck(render_pass_result.result, "failed to create render pass");
		m_RenderPass = render_pass_result.value;

		m_FrameBufferSet = FrameBufferSet::Create(RefPtr<RenderPass>(this));

		m_ColorValue.float32[0] = m_ClearSettings.color.x;
		m_ColorValue.float32[1] = m_ClearSettings.color.y;
		m_ColorValue.float32[2] = m_ClearSettings.color.z;
		m_ColorValue.float32[3] = m_ClearSettings.color.w;

		m_DepthValue.depth = 1.0f;
		m_DepthValue.stencil = 0u;

		m_ClearValues[0] = m_ColorValue;
		m_ClearValues[1] = m_DepthValue;
	}

	void VulkanRenderPass::Recreate()
	{
		Destroy();
		Init(m_Context, m_Settings);
	}

	void VulkanRenderPass::Destroy()
	{
		VkRenderAPI::GetVulkanDevice().destroyRenderPass(m_RenderPass);
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

	void VulkanRenderPass::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0)
		{
			__debugbreak();
		}

		m_Settings.Height = height;
		m_Settings.Width = width;
		Recreate();
		m_FrameBufferSet->ReSize(width, height);
	}

	RenderPassSettings& VulkanRenderPass::GetRenderPassSettings()
	{
		return m_Settings;
	}

	Ref<Texture2D> VulkanRenderPass::GetColorTexture(const TextureAttachmentSlot& attachmentIndex, uint32_t frameIndex)
	{
		return  m_FrameBufferSet->Get(frameIndex)->GetColorAttachment(attachmentIndex);
	}

	Ref<Texture2D> VulkanRenderPass::GetDepthTexture(uint32_t frameIndex)
	{
		return m_FrameBufferSet->Get(frameIndex)->GetDepthAttachment();

	}


	VulkanRenderPass::~VulkanRenderPass()
	{
	}

	vk::RenderPassBeginInfo VulkanRenderPass::GetRenderBeginInfo()
	{
		if (m_Settings.IsSwapchainRenderPass)
		{
			m_CurrentFramebuffer = RenderCommand::GetSwapChain()->GetCurrentFrameBuffer().As<VulkanFramebuffer>();
		}
		else
		{
			m_CurrentFramebuffer = m_FrameBufferSet->Get(RenderCommand::GetCurrentFrame()).As<VulkanFramebuffer>();
		}

		
		m_RenderPassBeginInfo.sType = vk::StructureType::eRenderPassBeginInfo;
		m_RenderPassBeginInfo.renderPass = m_RenderPass;
		m_RenderPassBeginInfo.framebuffer = m_CurrentFramebuffer->GetFramebufferHandle();
		m_RenderPassBeginInfo.renderArea.offset = vk::Offset2D{0, 0};
		m_RenderPassBeginInfo.renderArea.extent = vk::Extent2D{m_Settings.Width, m_Settings.Height};
		m_RenderPassBeginInfo.clearValueCount = 2;
		m_RenderPassBeginInfo.pClearValues = m_ClearValues.data();


		return m_RenderPassBeginInfo;
	}
}
