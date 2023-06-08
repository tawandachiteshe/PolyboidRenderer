#include "boidpch.h"
#include "VulkanTexture2D.h"
#include <vulkan/vulkan_raii.hpp>
#include "Utils/VulkanDevice.h"
#include "Utils/VulkanAllocatorInstance.h"
#include <vma/vk_mem_alloc.h>

#include "imgui_impl_vulkan.h"
#include "VkRenderAPI.h"
#include "VulkanCommandBufferSet.h"
#include "VulkanImage2D.h"
#include "VulkanSamplerState.h"
#include "VulkanStagingBuffer.h"
#include "Engine/Renderer/GraphicsBackend.h"
#include "Engine/Renderer/RenderCommand.h"
#include "Utils/Common.h"
#include "stb/stb_image.h"


namespace Polyboid
{
	void VulkanTexture2D::Init(const VkRenderAPI* context, const TextureSettings& settings, const void* data)
	{
		vk::Device device = (*context->GetDevice());
		VmaAllocator allocator = (*context->GetAllocator());

		ImageSettings imageSettings;
		imageSettings.height = settings.Height;
		imageSettings.width = settings.Width;
		imageSettings.sampleCount = settings.sampleCount;
		imageSettings.format = settings.sizedFormat;
		imageSettings.generateMips = settings.generateMips;
		imageSettings.usage = settings.usage;


		if (data)
		{
			auto size = imageSettings.width * imageSettings.height * 4;
			m_Image = CreateRef<VulkanImage2D>(context, imageSettings);

			auto staging = CreateRef<VulkanStagingBuffer>(context, data, size);

			Ref<VulkanCommandBufferSet> cmdList = CommandBufferSet::Create({ 1 }).As<VulkanCommandBufferSet>();

			const auto& cmdBuffer = cmdList->GetCommandBufferAt(0);
			cmdBuffer->Begin();
			cmdBuffer->TransitionImageLayout(m_Image.As<Image2D>(), ImageLayout::TransferDstOptimal);
			cmdBuffer->CopyBufferToImage2D(staging.As<StagingBuffer>(), m_Image.As<Image2D>());
			cmdBuffer->TransitionImageLayout(m_Image.As<Image2D>(), ImageLayout::ShaderReadOptimal);
			cmdBuffer->End();
			RenderCommand::GetGraphicsBackend()->SubmitOneTimeWork(cmdBuffer);

			staging->Destroy();
			cmdList->Destroy();

		}
		else
		{
			m_Image = CreateRef<VulkanImage2D>(context, imageSettings);
		}


		if (!settings.path.empty())
		{

			std::string imagePath = settings.path;
			int32_t w = 0, h = 0, c = 0;
			uint8_t* pixels = stbi_load(imagePath.data(), &w, &h, &c, 4);
			auto size = w * h * 4;
			imageSettings.width = static_cast<uint32_t>(w);
			imageSettings.height = static_cast<uint32_t>(h);
			m_Image = CreateRef<VulkanImage2D>(context, imageSettings);

			Ref<VulkanStagingBuffer> staging = CreateRef<VulkanStagingBuffer>(context, pixels, size);
			Ref<VulkanCommandBufferSet> cmdList = CommandBufferSet::Create({ 1 }).As<VulkanCommandBufferSet>();

			const auto& cmdBuffer = cmdList->GetCommandBufferAt(0);
			cmdBuffer->Begin();

			cmdBuffer->TransitionImageLayout(m_Image.As<Image2D>(), ImageLayout::TransferDstOptimal);
			cmdBuffer->CopyBufferToImage2D(staging.As<StagingBuffer>(), m_Image.As<Image2D>());
			cmdBuffer->TransitionImageLayout(m_Image.As<Image2D>(), ImageLayout::ShaderReadOptimal);

			cmdBuffer->End();
			RenderCommand::GetGraphicsBackend()->SubmitOneTimeWork(cmdBuffer);


			stbi_image_free(pixels);

			staging->Destroy();
			cmdList->Destroy();

		}
		// char* stats = nullptr;
		// vmaBuildStatsString(allocator, &stats, true);
		//
		// vmaFreeStatsString(allocator, stats);

		vk::ImageViewCreateInfo createViewInfo;
		createViewInfo.flags = vk::ImageViewCreateFlags();
		createViewInfo.sType = vk::StructureType::eImageViewCreateInfo;
		createViewInfo.viewType = vk::ImageViewType::e2D;
		createViewInfo.format = Utils::ConvertToVulkanFormat(settings.sizedFormat);
		createViewInfo.image = std::any_cast<vk::Image>(m_Image->GetHandle());
		createViewInfo.components = vk::ComponentMapping();
		createViewInfo.subresourceRange.aspectMask = Utils::ImageFormatToAspectBits(settings.sizedFormat); //vk::ImageAspectFlagBits::eColor;
		createViewInfo.subresourceRange.baseMipLevel = 0;
		createViewInfo.subresourceRange.levelCount = 1;
		createViewInfo.subresourceRange.baseArrayLayer = 0;
		createViewInfo.subresourceRange.layerCount = 1;

		auto [createViewResult, view] = device.createImageView(createViewInfo);
		vk::resultCheck(createViewResult, "Failed to create image view");

		m_View = view;



		if (data || !settings.path.empty() || settings.usage == ImageUsage::ColorAttachmentSampling || settings.usage == ImageUsage::DepthStencilAttachmentSampling)
		{
			SamplerSettings samplerSettings;
			samplerSettings.wrap = TextureWrap::ClampToBorder;
			samplerSettings.magFilter = MagFilterMode::Linear;
			samplerSettings.minFilter = MinFilterMode::Linear;
			samplerSettings.minLod = -1000;
			samplerSettings.magLod = 1000;

			Ref<VulkanSamplerState> sampler = CreateRef<VulkanSamplerState>(context, samplerSettings);
			m_SamplerState = sampler;
			auto vulkanSampler = std::any_cast<vk::Sampler>(sampler->GetSamplerHandle());


			m_ImageDescriptorInfo.sampler = vulkanSampler;
			m_ImageDescriptorInfo.imageView = view;
			m_ImageDescriptorInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
		}
	}

	void VulkanTexture2D::Recreate()
	{
		Destroy();
		Init(m_Context, m_Settings, m_Data);
	}

	VulkanTexture2D::VulkanTexture2D(const VkRenderAPI* context, const TextureSettings& settings, const void* data): m_Context(context), m_Settings(settings), m_Data(data)
	{
		Init(context, settings, data);
	}

	VulkanTexture2D::VulkanTexture2D(const VkRenderAPI* context, const vk::Image& imageHandle): m_Context(context), m_ImageMemory(nullptr)
	{
		vk::Device device = (*context->GetDevice());
		
		m_VulkanImage = imageHandle;

		//vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);

		vk::ImageViewCreateInfo createViewInfo;
		createViewInfo.flags = vk::ImageViewCreateFlags();
		createViewInfo.sType = vk::StructureType::eImageViewCreateInfo;
		createViewInfo.viewType = vk::ImageViewType::e2D;
		createViewInfo.format = vk::Format::eB8G8R8A8Unorm;
		createViewInfo.image = imageHandle;
		createViewInfo.components = vk::ComponentMapping();
		createViewInfo.subresourceRange = vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);

		auto [createViewResult, view] = device.createImageView(createViewInfo);
		vk::resultCheck(createViewResult, "Failed to create image view");

		m_View = view;

		VkImageView v = view;
		spdlog::info("Image View {}", (uint64_t)v);
	}

	void VulkanTexture2D::Destroy()
	{

		vk::Device device = (*m_Context->GetDevice());
		if (m_View != vk::ImageView(nullptr))
		{
			device.destroyImageView(m_View);
			m_View = nullptr;
		}

		if (m_Image.Get())
		{
			m_Image->Destroy();
		}

		if (m_SamplerState.Get())
		{
			m_SamplerState->Destroy();
		}

	
	}

	vk::DescriptorImageInfo VulkanTexture2D::GetVulkanDescriptorImageInfo()
	{
		m_ImageDescriptorInfo.sampler = std::any_cast<vk::Sampler>(m_SamplerState->GetSamplerHandle());
		m_ImageDescriptorInfo.imageView = m_View;
		m_ImageDescriptorInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;

		return m_ImageDescriptorInfo;
	}


	void VulkanTexture2D::Bind(uint32_t slot)
	{
	}

	void VulkanTexture2D::UnBind()
	{
	}

	std::any VulkanTexture2D::GetViewHandle()
	{
		return m_View;
	}

	std::any VulkanTexture2D::GetSamplerHandle()
	{
		return m_SamplerState->GetSamplerHandle();
	}

	std::any VulkanTexture2D::GetImGuiTexture()
	{
		return (ImTextureID)m_ImguiDS;
	}


	void VulkanTexture2D::SetData(const void* data, uint32_t size)
	{
	}

	VulkanTexture2D::~VulkanTexture2D()
	{
		spdlog::info("Reloaded hahha");
	}
}
