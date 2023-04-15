#include "boidpch.h"
#include "VulkanFramebuffer.h"

#include <spdlog/spdlog.h>

#include "VkRenderAPI.h"
#include "Utils/VulkanDevice.h"
#include "Engine/Renderer/Texture.h"

namespace Polyboid
{
	void VulkanFramebuffer::Init(const VkRenderAPI* context, const FramebufferSettings& settings)
	{
		if (m_RenderPassPtr)
		{
			vk::Device device = (*m_Context->GetDevice());
			std::vector<vk::ImageView> attachments;
			for (const auto& texture : m_Settings.textures)
			{
				attachments.push_back(std::any_cast<vk::ImageView>(texture->GetHandle()));
			}

			vk::FramebufferCreateInfo createInfo;
			createInfo.sType = vk::StructureType::eFramebufferCreateInfo;
			createInfo.renderPass = m_RenderPassPtr->m_RenderPass;
			createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			createInfo.height = m_Settings.height;
			createInfo.width = m_Settings.width;
			createInfo.pAttachments = attachments.data();
			createInfo.layers = 1;


			auto [result, framebuffer] = device.createFramebuffer(createInfo);
			vk::resultCheck(result, "Failed to create framebuffer");
			m_Framebuffer = framebuffer;
		}
	
	}

	void VulkanFramebuffer::Destroy(vk::Device device)
	{
		device.destroyFramebuffer(m_Framebuffer);
	}

	VulkanFramebuffer::VulkanFramebuffer(const VkRenderAPI* context, const FramebufferSettings& settings):
		m_Settings(settings), m_Context(context)
	{

	}


	VulkanFramebuffer::VulkanFramebuffer(const VkRenderAPI* context, const FramebufferSettings& settings,
		RenderPass* renderPass): m_RenderPassPtr(reinterpret_cast<VulkanRenderPass*>(renderPass)), m_Settings(settings), m_Context(context)
	{
		Init(context, m_Settings);
	}


	void VulkanFramebuffer::AttachRenderbuffer(const Ref<Renderbuffer>& renderbuffer, const TextureAttachmentSlot& slot)
	{
	}

	void VulkanFramebuffer::AttachTexture(const Ref<Texture>& texture, const TextureAttachmentSlot& slot)
	{
	}

	void VulkanFramebuffer::AttachTexture3D(const Ref<Texture3D>& texture, const TextureAttachmentSlot& slot)
	{
	}

	VulkanFramebuffer::~VulkanFramebuffer()
	{
	}

	void VulkanFramebuffer::Clear(TextureAttachmentSlot attachment, const ClearSettings& settings)
	{
	}

	void VulkanFramebuffer::Clear(const ClearSettings& settings)
	{
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


		Init(m_Context, m_Settings);
	}
}
