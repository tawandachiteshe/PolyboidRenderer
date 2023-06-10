#include "boidpch.h"
#include "VulkanTexture3D.h"

#include "VkRenderAPI.h"
#include "VulkanCommandBuffer.h"
#include "VulkanCommandBufferSet.h"
#include "VulkanImage2D.h"
#include "VulkanSamplerState.h"
#include "VulkanStagingBuffer.h"
#include "Engine/Renderer/RenderCommand.h"
#include "Engine/Renderer/SamplerState.h"
#include "Engine/Renderer/Texture2D.h"
#include "Platform/VulkanImage3D.h"
#include "Utils/Common.h"


namespace Polyboid
{

	

	VulkanTexture3D::VulkanTexture3D(const void* data, const TextureSettings& settings)
	{
		vk::Device device = VkRenderAPI::GetVulkanDevice();

		ImageSettings imageSettings;
		imageSettings.height = settings.Height;
		imageSettings.width = settings.Width;
		imageSettings.depth = settings.Depth;
		imageSettings.sampleCount = settings.sampleCount;
		imageSettings.format = settings.sizedFormat;
		imageSettings.generateMips = settings.generateMips;
		imageSettings.mipCount = settings.mipCount;
		imageSettings.usage = settings.usage;
		imageSettings.layerCount = 6;
		const auto context = reinterpret_cast<VkRenderAPI*>(RenderAPI::Get());


		if (data)
		{
			auto size = imageSettings.width * imageSettings.height * 4 * 6;
			m_Image = CreateRef<VulkanImage2D>(context, imageSettings);

			auto staging = CreateRef<VulkanStagingBuffer>(context, data, size);

			Ref<VulkanCommandBufferSet> cmdList = CommandBufferSet::Create({ 1 }).As<VulkanCommandBufferSet>();

		

			std::vector<vk::BufferImageCopy> bufferCopyRegions;
			uint32_t offset = 0;

			for (uint32_t face = 0; face < 6; face++)
			{
	
				vk::BufferImageCopy bufferCopyRegion{};
				bufferCopyRegion.imageSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
				bufferCopyRegion.imageSubresource.mipLevel = 0;
				bufferCopyRegion.imageSubresource.baseArrayLayer = face;
				bufferCopyRegion.imageSubresource.layerCount = 1;
				bufferCopyRegion.imageExtent = vk::Extent3D{ settings.Width, settings.Height, 1 };
				bufferCopyRegion.bufferOffset = offset;
				
				bufferCopyRegions.push_back(bufferCopyRegion);

				offset += imageSettings.width * imageSettings.height * 4;
			}

			const auto& cmdBuffer = cmdList->GetCommandBufferAt(0).As<VulkanCommandBuffer>();
			cmdBuffer->Begin();
			cmdBuffer->TransitionImageLayout(m_Image.As<Image2D>(), ImageLayout::TransferDstOptimal, 6);
			cmdBuffer->VulkanCopyBufferToCubemap(staging.As<StagingBuffer>(), m_Image.As<Image2D>(), { bufferCopyRegions });

			if (settings.generateMips)
			{
				cmdBuffer->TransitionImageLayout(m_Image.As<Image2D>(), ImageLayout::TransferSrcOptimal, 1, settings.mipCount);
			}
			else
			{
				cmdBuffer->TransitionImageLayout(m_Image.As<Image2D>(), ImageLayout::ShaderReadOptimal, 6);
			}

			cmdBuffer->End();
			RenderCommand::GetGraphicsBackend()->SubmitOneTimeWork(cmdBuffer.As<CommandBuffer>());

			staging->Destroy();
			cmdList->Destroy();

		}
		else
			if (!settings.path.empty())
			{
				__debugbreak();
				// std::string imagePath = settings.path;
				// int32_t w = 0, h = 0, c = 0;
				// uint8_t* pixels = stbi_load(imagePath.data(), &w, &h, &c, 4);
				// auto size = w * h * 4;
				// imageSettings.width = static_cast<uint32_t>(w);
				// imageSettings.height = static_cast<uint32_t>(h);
				// m_Image = CreateRef<VulkanImage2D>(context, imageSettings);
				//
				// Ref<VulkanStagingBuffer> staging = CreateRef<VulkanStagingBuffer>(context, pixels, size);
				// Ref<VulkanCommandBufferSet> cmdList = CommandBufferSet::Create({ 1 }).As<VulkanCommandBufferSet>();
				//
				// const auto& cmdBuffer = cmdList->GetCommandBufferAt(0);
				// cmdBuffer->Begin();
				//
				// cmdBuffer->TransitionImageLayout(m_Image.As<Image2D>(), ImageLayout::TransferDstOptimal);
				// cmdBuffer->CopyBufferToImage2D(staging.As<StagingBuffer>(), m_Image.As<Image2D>());
				//
				// if (m_Settings.generateMips)
				// {
				// 	cmdBuffer->TransitionImageLayout(m_Image.As<Image2D>(), ImageLayout::TransferSrcOptimal);
				// }
				// else
				// {
				// 	cmdBuffer->TransitionImageLayout(m_Image.As<Image2D>(), ImageLayout::ShaderReadOptimal);
				// }
				//
				// cmdBuffer->End();
				// RenderCommand::GetGraphicsBackend()->SubmitOneTimeWork(cmdBuffer);
				//
				//
				// stbi_image_free(pixels);
				//
				// staging->Destroy();
				// cmdList->Destroy();

			}
			else
			{
				m_Image = CreateRef<VulkanImage2D>(context, imageSettings);
			}
		// char* stats = nullptr;
		// vmaBuildStatsString(allocator, &stats, true);
		//
		// vmaFreeStatsString(allocator, stats);

		vk::ImageViewCreateInfo createViewInfo;
		createViewInfo.flags = vk::ImageViewCreateFlags();
		createViewInfo.sType = vk::StructureType::eImageViewCreateInfo;
		createViewInfo.viewType = vk::ImageViewType::eCube;
		createViewInfo.format = Utils::ConvertToVulkanFormat(settings.sizedFormat);
		createViewInfo.image = std::any_cast<vk::Image>(m_Image->GetHandle());
		createViewInfo.components = vk::ComponentMapping();
		createViewInfo.subresourceRange.aspectMask = Utils::ImageFormatToAspectBits(settings.sizedFormat); //vk::ImageAspectFlagBits::eColor;
		createViewInfo.subresourceRange.baseMipLevel = 0;
		createViewInfo.subresourceRange.levelCount = settings.mipCount;
		createViewInfo.subresourceRange.baseArrayLayer = 0;
		createViewInfo.subresourceRange.layerCount = 6;


		if (settings.generateMips)
		{
			//uint32_t mipCount = glm::floor(glm::log2(float(glm::min(512, 512)))) + 1u;


			Ref<VulkanCommandBufferSet> cmdList = CommandBufferSet::Create({ 1 }).As<VulkanCommandBufferSet>();

			const auto& cmdBuffer = cmdList->GetCommandBufferAt(0).As<VulkanCommandBuffer>();
			cmdBuffer->Begin();

			cmdBuffer->VulkanGenerateMips(createViewInfo.image, imageSettings.width, imageSettings.height, imageSettings.mipCount);

			cmdBuffer->End();
			RenderCommand::GetGraphicsBackend()->SubmitOneTimeWork(cmdBuffer.As<CommandBuffer>());


			cmdList->Destroy();

		}

		auto [createViewResult, view] = device.createImageView(createViewInfo);
		vk::resultCheck(createViewResult, "Failed to create image view");

		m_View = view;




		if (data || !settings.path.empty() || settings.usage == ImageUsage::ColorAttachmentSampling || settings.usage == ImageUsage::DepthStencilAttachmentSampling)
		{
			SamplerSettings samplerSettings;
			samplerSettings.wrap = TextureWrap::ClampToBorder;
			samplerSettings.magFilter = MagFilterMode::Linear;
			samplerSettings.minFilter = MinFilterMode::Linear;
			samplerSettings.minLod = 0;
			if (settings.generateMips)
			{
				samplerSettings.mipModeFilter = MinFilterMode::Linear;
			}

			if (settings.generateMips)
			{
				samplerSettings.magLod = static_cast<float>(settings.mipCount);
			}
			else
			{
				samplerSettings.magLod = 1000;
			}


			Ref<VulkanSamplerState> sampler = CreateRef<VulkanSamplerState>(context, samplerSettings);
			m_SamplerState = sampler;
			auto vulkanSampler = std::any_cast<vk::Sampler>(sampler->GetSamplerHandle());


			m_ImageDescriptorInfo.sampler = vulkanSampler;
			m_ImageDescriptorInfo.imageView = view;
			m_ImageDescriptorInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
		}
	}

	void VulkanTexture3D::Destroy()
	{
		vk::Device device = VkRenderAPI::GetVulkanDevice();
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

	void VulkanTexture3D::Recreate()
	{
	}

	void VulkanTexture3D::Bind(uint32_t slot)
	{
	}

	void VulkanTexture3D::UnBind()
	{
	}

	void VulkanTexture3D::SetData(const void** data, uint32_t size)
	{
	}

	std::any VulkanTexture3D::GetViewHandle()
	{
		return m_View;
	}

	std::any VulkanTexture3D::GetSamplerHandle()
	{
		return m_Image;
	}

	vk::DescriptorImageInfo VulkanTexture3D::GetVulkanDescriptorImageInfo()
	{
		return m_ImageDescriptorInfo;
	}
}
