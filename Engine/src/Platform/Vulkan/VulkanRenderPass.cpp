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
		m_Framebuffer = framebuffer;
	}

	Ref<Framebuffer> VulkanRenderPass::GetFramebuffer()
	{
		return m_Framebuffer;
	}

	VulkanRenderPass::VulkanRenderPass(const VkRenderAPI* context, const RenderPassSettings& settings): m_Settings(
		settings)
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
			case RenderPassType::Present: attachmentDescription.finalLayout = vk::ImageLayout::ePresentSrcKHR;
				break;
			case RenderPassType::ColorAttachment: break;
			case RenderPassType::Copy: attachmentDescription.finalLayout = vk::ImageLayout::eTransferDstOptimal;
				break;
			}


			vk::AttachmentReference colorAttachmentRef{};
			colorAttachmentRef.attachment = count;
			colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

			attachmentments.push_back(attachmentDescription);
			attachmentmentsRefs.push_back(colorAttachmentRef);

			count++;
		}

		auto depthAttachment = vk::AttachmentDescription()
		                       .setFormat(vk::Format::eD24UnormS8Uint)
		                       .setSamples(vk::SampleCountFlagBits::e1)
		                       .setLoadOp(vk::AttachmentLoadOp::eClear)
		                       .setStoreOp(vk::AttachmentStoreOp::eDontCare)
		                       .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
		                       .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
		                       .setInitialLayout(vk::ImageLayout::eUndefined)
		                       .setFinalLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

		vk::AttachmentReference depthAttachmentRef{};
		depthAttachmentRef.attachment = count;
		depthAttachmentRef.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;


		attachmentments.push_back(depthAttachment);
		count += 1;


		vk::SubpassDescription subpass{};
		subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
		subpass.colorAttachmentCount = static_cast<uint32_t>(attachmentmentsRefs.size());
		subpass.pColorAttachments = attachmentmentsRefs.data();
		subpass.pDepthStencilAttachment = &depthAttachmentRef;

		vk::PipelineStageFlags stages = vk::PipelineStageFlagBits::eEarlyFragmentTests |
			vk::PipelineStageFlagBits::eLateFragmentTests;

		std::array<vk::SubpassDependency, 2> const dependencies = {
			
			vk::SubpassDependency() // Image layout transition
			.setSrcSubpass(VK_SUBPASS_EXTERNAL)
			.setDstSubpass(0)
			.setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
			.setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
			.setSrcAccessMask(vk::AccessFlagBits())
			.setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eColorAttachmentRead)
			.setDependencyFlags(vk::DependencyFlags()),
			vk::SubpassDependency() // Depth buffer is shared between swapchain images
			.setSrcSubpass(VK_SUBPASS_EXTERNAL)
			.setDstSubpass(0)
			.setSrcStageMask(stages)
			.setDstStageMask(stages)
			.setSrcAccessMask(vk::AccessFlagBits::eDepthStencilAttachmentWrite)
			.setDstAccessMask(
				vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite)
			.setDependencyFlags(vk::DependencyFlags())
		};


		vk::RenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = vk::StructureType::eRenderPassCreateInfo;
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachmentments.size());
		renderPassInfo.pAttachments = attachmentments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = dependencies.size();
		renderPassInfo.pDependencies = dependencies.data();

		auto [result, renderpass] = device.createRenderPass(renderPassInfo);
		vk::resultCheck(result, "Failed to create render pass");
		m_RenderPass = renderpass;
	}

	void VulkanRenderPass::Destroy(vk::Device device)
	{
		device.destroyRenderPass(m_RenderPass);
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
}
