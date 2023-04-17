#include "boidpch.h"
#include "VulkanFramebuffer.h"

#include <spdlog/spdlog.h>

#include "VkRenderAPI.h"
#include "VkSwapChain.h"
#include "Engine/Engine/Application.h"
#include "Utils/VulkanDevice.h"
#include "Engine/Renderer/Texture.h"
#include "Platform/Vulkan/VulkanTexture2D.h"

namespace Polyboid
{
	void VulkanFramebuffer::Init(const VkRenderAPI* context, const FramebufferSettings& settings)
	{
		if (m_RenderPassPtr)
		{
			vk::Device device = (*m_Context->GetDevice());
			std::vector<vk::ImageView> frameBufferImages;
			m_FrameBuffers.resize(1);

			if (settings.IsSwapChainUsage)
			{
				m_Textures = VkSwapChain::CreateSwapchainTextures();

				m_FrameBuffers.resize(m_Textures.size());
				for (const auto& texture : m_Textures)
				{
					frameBufferImages.push_back(std::any_cast<vk::ImageView>(texture->GetHandle()));
				}

			}
			else
			{
				for (auto textureSlot : settings.attachmentSlots)
				{

					TextureSettings textureSettings{};
					textureSettings.Width = settings.width;
					textureSettings.Height = settings.height;
					textureSettings.sizedFormat = textureSlot.format;
					VkRenderAPI* api = const_cast<VkRenderAPI*>(context);
					m_AttachmentTextures[textureSlot.slot] = std::reinterpret_pointer_cast<VulkanTexture2D>(api->CreateTexture2D(textureSettings));
				}
			}


			int count = 0;
			for (auto view : frameBufferImages)
			{
				vk::ImageView imageView = view;

				vk::FramebufferCreateInfo createInfo;
				createInfo.sType = vk::StructureType::eFramebufferCreateInfo;
				createInfo.renderPass = m_RenderPassPtr->m_RenderPass;
				createInfo.attachmentCount = 1;
				createInfo.height = m_Settings.height;
				createInfo.width = m_Settings.width;
				createInfo.pAttachments = &imageView;
				createInfo.layers = 1;


				auto [result, framebuffer] = device.createFramebuffer(createInfo);
				vk::resultCheck(result, "Failed to create framebuffer");

				m_FrameBuffers[count] = framebuffer;
				count++;
			}
		
		}
	
	}

	void VulkanFramebuffer::Destroy(vk::Device device)
	{

		for (auto texture : m_Textures)
		{
			texture->Destroy();
		}


		for (auto framebuffer : m_FrameBuffers)
		{
			device.destroyFramebuffer(framebuffer);
			m_FrameBuffers.clear();
		}

	

		
	}

	VulkanFramebuffer::VulkanFramebuffer(const VkRenderAPI* context, const FramebufferSettings& settings):
		m_Settings(settings), m_Context(context)
	{

	}


	VulkanFramebuffer::VulkanFramebuffer(const VkRenderAPI* context, const FramebufferSettings& settings,
		RenderPass* renderPass): m_Settings(settings), m_Context(context),
		                         m_RenderPassPtr(reinterpret_cast<VulkanRenderPass*>(renderPass))
	{
		Init(context, m_Settings);
	}

	vk::Framebuffer VulkanFramebuffer::GetFramebufferHandle(uint32_t index)
	{
		return m_FrameBuffers.at(index);
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
