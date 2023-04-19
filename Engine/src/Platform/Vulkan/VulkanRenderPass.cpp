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

		uint32_t count = 0;

		std::vector<vk::AttachmentDescription> attachmentments;
		std::vector<vk::AttachmentReference> attachmentmentsRefs;

		for (auto& attachment : settings.TextureAttachments)
		{

			vk::AttachmentDescription attachmentDescription{};
			attachmentDescription.format = Utils::ConvertToVulkanFormat(attachment.format);
			attachmentDescription.samples = vk::SampleCountFlagBits::e1;
			attachmentDescription.loadOp = vk::AttachmentLoadOp::eClear;
			attachmentDescription.storeOp = vk::AttachmentStoreOp::eStore;

			attachmentDescription.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
			attachmentDescription.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
			attachmentDescription.initialLayout = vk::ImageLayout::eUndefined;


			switch (settings.type)
			{
			case RenderPassType::Present: attachmentDescription.finalLayout = vk::ImageLayout::ePresentSrcKHR; break;
			case RenderPassType::ColorAttachment: break;
			case RenderPassType::Copy: attachmentDescription.finalLayout = vk::ImageLayout::eTransferDstOptimal; break;
			}


			vk::AttachmentReference colorAttachmentRef{};
			colorAttachmentRef.attachment = count;
			colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

			attachmentments.push_back(attachmentDescription);
			attachmentmentsRefs.push_back(colorAttachmentRef);

			count++;
		}


		vk::SubpassDescription subpass{};
		subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
		subpass.colorAttachmentCount = static_cast<uint32_t>(attachmentmentsRefs.size());
		subpass.pColorAttachments = attachmentmentsRefs.data();
		

		vk::SubpassDependency dependency = {};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
		dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
		dependency.srcAccessMask = vk::AccessFlagBits::eNone;
		dependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;


		vk::RenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = vk::StructureType::eRenderPassCreateInfo;
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachmentments.size());
		renderPassInfo.pAttachments = attachmentments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		auto [result, renderpass] = device.createRenderPass(renderPassInfo);
		vk::resultCheck(result, "Failed to create render pass");
		m_RenderPass = renderpass;


		FramebufferSettings framebufferSettings;
		framebufferSettings.height = settings.Height;
		framebufferSettings.width = settings.Width;
		framebufferSettings.attachmentSlots = m_Settings.TextureAttachments;
		framebufferSettings.IsSwapChainUsage = settings.IsSwapchainRenderPass;

		m_Framebuffer = std::make_shared<VulkanFramebuffer>(context, framebufferSettings, this);
	

		m_ColorValue.float32[0] = m_ClearSettings.color.x;
		m_ColorValue.float32[1] = m_ClearSettings.color.y;
		m_ColorValue.float32[2] = m_ClearSettings.color.z;
		m_ColorValue.float32[3] = m_ClearSettings.color.w;

		m_ClearValues[0] = m_ColorValue;

	}

	void VulkanRenderPass::Destroy(vk::Device device)
	{
	
		device.destroyRenderPass(m_RenderPass);
		m_Framebuffer->Destroy(device);
	}


	void VulkanRenderPass::AttachTexture(TextureAttachmentSlot attachment, Ref<Texture> texture)
	{
		vk::AttachmentDescription attachmentDescription{};
	}

	Ref<Texture> VulkanRenderPass::GetTexture(TextureAttachmentSlot attachment)
	{
		return nullptr;
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

	void VulkanRenderPass::GenerateMipMaps()
	{
	}

	VulkanRenderPass::~VulkanRenderPass()
	{
	}

	vk::RenderPassBeginInfo VulkanRenderPass::GetRenderBeginInfo(uint32_t imageIndex)
	{

		m_RenderPassBeginInfo.sType = vk::StructureType::eRenderPassBeginInfo;

		if (m_Settings.type == RenderPassType::Present)
		{
			m_RenderPassBeginInfo.renderPass = m_RenderPass;
			m_RenderPassBeginInfo.framebuffer = m_Framebuffer->GetFramebufferHandle(imageIndex);
		}
		else
		{
			auto vkFramebuffer = std::reinterpret_pointer_cast<VulkanFramebuffer>(m_Framebuffer);
			m_RenderPassBeginInfo.renderPass = m_RenderPass;
			m_RenderPassBeginInfo.framebuffer = m_Framebuffer->GetFramebufferHandle();
		}

		m_RenderPassBeginInfo.renderArea.offset = vk::Offset2D{ 0, 0 };
		m_RenderPassBeginInfo.renderArea.extent = vk::Extent2D{ m_Settings.Width, m_Settings.Height };


		m_RenderPassBeginInfo.clearValueCount = 1;
		m_RenderPassBeginInfo.pClearValues = m_ClearValues.data();

		return m_RenderPassBeginInfo;

	}
}
