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
		auto device = context->GetDevice()->GetVulkanDevice();


		if (m_RenderPassPtr)
		{
			std::vector<vk::ImageView> imageViews;

			for (auto textures : m_Textures)
			{
				imageViews.push_back(std::any_cast<vk::ImageView>(textures->GetHandle()));
			}

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

	void VulkanFramebuffer::Destroy(vk::Device device)
	{

		for (auto texture : m_Textures)
		{
			texture->Destroy();
		}

		device.destroyFramebuffer(m_FrameBuffer);
	}

	VulkanFramebuffer::VulkanFramebuffer(const VkRenderAPI* context, const FramebufferSettings& settings):
		m_Settings(settings), m_Context(context)
	{

	}



	VulkanFramebuffer::VulkanFramebuffer(const VkRenderAPI* context, const FramebufferSettings& settings,
		const VulkanRenderPass* renderPass, std::vector<Ref<VulkanTexture2D>>& textures): m_Context(context),
		m_RenderPassPtr(renderPass), m_Textures(textures)
	{
		Init(context, settings);
	}

	vk::Framebuffer VulkanFramebuffer::GetFramebufferHandle()
	{
		return m_FrameBuffer;
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
