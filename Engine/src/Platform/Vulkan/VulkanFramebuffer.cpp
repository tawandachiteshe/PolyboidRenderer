#include "boidpch.h"
#include "VulkanFramebuffer.h"

#include <spdlog/spdlog.h>

#include "VkRenderAPI.h"
#include "Utils/VulkanDevice.h"
#include "Engine/Renderer/Texture.h"
#include "Platform/Vulkan/VulkanTexture2D.h"

namespace Polyboid
{
	void VulkanFramebuffer::SwapChainInit(const VkRenderAPI* context, const FramebufferSettings& settings)
	{
		auto device = context->GetDevice()->GetVulkanDevice();


		if (m_RenderPassPtr)
		{
			std::vector<vk::ImageView> imageViews;
			imageViews.push_back(std::any_cast<vk::ImageView>(m_Textures[0]->GetViewHandle()));
			imageViews.push_back(std::any_cast<vk::ImageView>(m_Textures[1]->GetViewHandle()));
			


			vk::FramebufferCreateInfo createInfo;
			createInfo.sType = vk::StructureType::eFramebufferCreateInfo;
			createInfo.renderPass = m_RenderPassPtr->m_RenderPass;
			createInfo.attachmentCount = static_cast<uint32_t>(imageViews.size());
			createInfo.height = settings.height;
			createInfo.width = settings.width;
			createInfo.pAttachments = imageViews.data();
			createInfo.layers = 1;


			auto [result, framebuffer] = device.createFramebuffer(createInfo);
			vk::resultCheck(result, "Failed to create framebuffer");



			m_FrameBuffer = framebuffer;
		}
	
	}

	void VulkanFramebuffer::Init(const VkRenderAPI* context, const Ref<VulkanRenderPass>& renderPass)
	{

		auto device = context->GetDevice()->GetVulkanDevice();

		std::vector<vk::ImageView> imageViews;
		auto renderPassSettings = renderPass->GetRenderPassSettings();

		for (auto& attachmentSlot : renderPassSettings.TextureAttachments)
		{


			TextureSettings attachmentSettings{};
			attachmentSettings.Height = renderPassSettings.Height;
			attachmentSettings.Width = renderPassSettings.Width;
			attachmentSettings.sizedFormat = attachmentSlot.format;
			attachmentSettings.usage = ImageUsage::ColorAttachmentSampling;

			auto colorTexture = CreateRef<VulkanTexture2D>(context, attachmentSettings);
			imageViews.push_back(std::any_cast<vk::ImageView>(colorTexture->GetViewHandle()));
			m_Textures.push_back(colorTexture);
		}

		TextureSettings textureSettings{};
		textureSettings.Height = renderPassSettings.Height;
		textureSettings.Width = renderPassSettings.Width;
		textureSettings.usage = ImageUsage::DepthStencilAttachment;
		textureSettings.sizedFormat = EngineGraphicsFormats::Depth32FStencil8;

		auto depthTexture = CreateRef<VulkanTexture2D>(context, textureSettings);
		imageViews.push_back(std::any_cast<vk::ImageView>(std::any_cast<vk::ImageView>(depthTexture->GetViewHandle())));
		m_Textures.push_back(depthTexture);

		vk::FramebufferCreateInfo createInfo;
		createInfo.sType = vk::StructureType::eFramebufferCreateInfo;
		createInfo.renderPass = std::any_cast<vk::RenderPass>(renderPass->GetHandle());
		createInfo.attachmentCount = static_cast<uint32_t>(imageViews.size());
		createInfo.height = renderPassSettings.Height;
		createInfo.width = renderPassSettings.Width;
		createInfo.pAttachments = imageViews.data();
		createInfo.layers = 1;

		auto [result, framebuffer] = device.createFramebuffer(createInfo);
		vk::resultCheck(result, "Failed to create framebuffer");
		m_FrameBuffer = framebuffer;

	}

	void VulkanFramebuffer::SwapchainRecreate()
	{
		Destroy(VkRenderAPI::GetVulkanDevice());
		SwapChainInit(m_Context, m_Settings);
	}

	void VulkanFramebuffer::Recreate()
	{
		Destroy(VkRenderAPI::GetVulkanDevice());
		Init(m_Context, m_RenderPass);
	}

	void VulkanFramebuffer::Destroy(vk::Device device)
	{

		for (auto texture : m_Textures)
		{
			texture->Destroy();
		}

		device.destroyFramebuffer(m_FrameBuffer);
	}

	VulkanFramebuffer::VulkanFramebuffer(const VkRenderAPI* context, const Ref<VulkanRenderPass>& renderPass):
		m_Context(context), m_RenderPass(renderPass)
	{
		Init(context, renderPass);
	}



	VulkanFramebuffer::VulkanFramebuffer(const VkRenderAPI* context, const FramebufferSettings& settings,
		const VulkanRenderPass* renderPass, std::vector<Ref<VulkanTexture2D>>& textures): m_Context(context),
		m_RenderPassPtr(renderPass), m_Textures(textures)
	{
		SwapChainInit(context, settings);
	}

	vk::Framebuffer VulkanFramebuffer::GetFramebufferHandle()
	{
		return m_FrameBuffer;
	}


	VulkanFramebuffer::~VulkanFramebuffer()
	{
	}

	Ref<Texture> VulkanFramebuffer::GetColorAttachment(TextureAttachmentSlot attachment)
	{
		if (m_Textures.empty())
		{
			__debugbreak();
		}

		return m_Textures.at(static_cast<uint32_t>(attachment)).As<Texture>();
	}


	void VulkanFramebuffer::ReSize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0)
		{
			spdlog::info("Can't resize buffer with lenght of zero");
			__debugbreak();
		}

		m_Settings.height = height;
		m_Settings.width = width;
		Recreate();
	}

	std::any VulkanFramebuffer::GetHandle()
	{
		return m_FrameBuffer;
	}
}
