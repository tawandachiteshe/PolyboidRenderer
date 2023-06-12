#include "boidpch.h"
#include "VulkanImage2D.h"

#include <vulkan/vulkan_raii.hpp>
#include "Utils/VulkanAllocatorInstance.h"
#include "vma/vk_mem_alloc.h"
#include "Utils/VulkanDevice.h"
#include "VkRenderAPI.h"
#include "VulkanCommandBuffer.h"
#include "VulkanCommandBufferSet.h"
#include "Engine/Renderer/CommandBufferSet.h"
#include "Engine/Renderer/RenderCommand.h"
#include "Utils/Common.h"


namespace Polyboid
{
	void VulkanImage2D::Init(const VkRenderAPI* context, const ImageSettings& imageSettings)
	{
		vk::Device device = (*context->GetDevice());
		VmaAllocator allocator = (*context->GetAllocator());

		vk::ImageCreateInfo createInfo{};
		if (imageSettings.layerCount == 6)
		{
			createInfo.flags = vk::ImageCreateFlagBits::eCubeCompatible;
		}
		else
		{
			createInfo.flags = vk::ImageCreateFlags();
		}
	
		createInfo.sType = vk::StructureType::eImageCreateInfo;
		createInfo.imageType = vk::ImageType::e2D;
		createInfo.extent.width = imageSettings.width;
		createInfo.extent.height = imageSettings.height;
		createInfo.extent.depth = 1;

		if (imageSettings.generateMips)
		{
			createInfo.mipLevels = imageSettings.mipCount;
		}
		else
		{
			createInfo.mipLevels = 1;
		}

		createInfo.arrayLayers = imageSettings.layerCount;
		createInfo.format = Utils::ConvertToVulkanFormat(imageSettings.format);
		createInfo.tiling = vk::ImageTiling::eOptimal;

		createInfo.initialLayout =  Utils::ConvertToVulkanLayout(imageSettings.layout);

		using enum vk::ImageUsageFlagBits;

		switch (imageSettings.usage)
		{
		case ImageUsage::Swapchain: break;
		case ImageUsage::ColorDepthStencilAttachment:createInfo.usage = eColorAttachment | eDepthStencilAttachment; break;
		case ImageUsage::Sampling: createInfo.usage = imageSettings.generateMips ? eTransferSrc | eSampled | eTransferDst : eSampled | eTransferDst; break;
		case ImageUsage::ColorAttachment: createInfo.usage = eColorAttachment; break;
		case ImageUsage::DepthStencilAttachment: createInfo.usage = eDepthStencilAttachment; break;
		case ImageUsage::TransferSrc: createInfo.usage = eTransferSrc; break;
		case ImageUsage::TransferDst: createInfo.usage = eTransferDst; break;
		case ImageUsage::ColorAttachmentSampling: createInfo.usage = imageSettings.generateMips ? eTransferSrc | eColorAttachment | eSampled : eColorAttachment | eSampled;  break;
		case ImageUsage::DepthStencilAttachmentSampling: createInfo.usage = eDepthStencilAttachment | eSampled;  break;
		case ImageUsage::StorageImage: createInfo.usage = eStorage | eTransferSrc | eSampled;
		default:;
		}

		createInfo.sharingMode = vk::SharingMode::eExclusive;
		createInfo.samples = vk::SampleCountFlagBits::e1;

		vk::ImageCreateInfo::NativeType vulkanImageInfo = createInfo;

		VmaAllocationCreateInfo allocInfo = {};
		allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
		// allocInfo.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
		allocInfo.priority = 1.0f;
		VmaAllocation allocation;


		vk::Image::NativeType vulkanImage;
		const auto result = vmaCreateImage(allocator, &vulkanImageInfo, &allocInfo, &vulkanImage, &allocation, nullptr);

		if (result != VK_SUCCESS)
		{
			__debugbreak();
		}

		m_Image = vulkanImage;
		m_ImageMemory = allocation;


		if (imageSettings.usage == ImageUsage::StorageImage)
		{


			Ref<VulkanCommandBufferSet> cmdList = CommandBufferSet::Create({ 1 }).As<VulkanCommandBufferSet>();

			const auto& cmdBuffer = cmdList->GetCommandBufferAt(0).As<VulkanCommandBuffer>();
			cmdBuffer->Begin();
			cmdBuffer->VulkanTransitionImageLayout(m_Image, vk::ImageLayout::eUndefined, vk::ImageLayout::eGeneral);

			cmdBuffer->End();
			RenderCommand::GetGraphicsBackend()->SubmitOneTimeWork(cmdBuffer.As<CommandBuffer>());

			cmdList->Destroy();
		}


		vk::ImageViewCreateInfo createViewInfo;
		createViewInfo.flags = vk::ImageViewCreateFlags();
		createViewInfo.sType = vk::StructureType::eImageViewCreateInfo;
		createViewInfo.viewType = vk::ImageViewType::e2D;
		createViewInfo.format = createInfo.format;
		createViewInfo.image = m_Image;
		
		createViewInfo.components = vk::ComponentMapping();
		createViewInfo.subresourceRange = vk::ImageSubresourceRange(Utils::ImageFormatToAspectBits(imageSettings.format), 0, 1, 0, 1);

		auto [createViewResult, view] = device.createImageView(createViewInfo);
		vk::resultCheck(createViewResult, "Failed to create image view");

		m_View = view;


		m_ImageDescInfo.imageLayout = imageSettings.usage == ImageUsage::StorageImage ? vk::ImageLayout::eGeneral : createInfo.initialLayout;
		m_ImageDescInfo.imageView = view;
	}

	void VulkanImage2D::Recreate()
	{
		Destroy();
		Init(m_Context, m_Settings);
	}

	vk::DescriptorImageInfo VulkanImage2D::GetVulkanDescriptorImageInfo()
	{
		return m_ImageDescInfo;
	}

	VulkanImage2D::VulkanImage2D(const VkRenderAPI* context, const ImageSettings& imageSettings): m_Context(context), m_Settings(imageSettings)
	{
		Init(context, m_Settings);
	}

	std::any VulkanImage2D::GetHandle()
	{
		return m_Image;
	}

	void VulkanImage2D::Destroy()
	{
		if (m_Image != vk::Image(nullptr) && m_ImageMemory != nullptr)
		{
			vk::Device device = (*m_Context->GetDevice());
			VmaAllocator allocator = (*m_Context->GetAllocator());

			vmaDestroyImage(allocator, m_Image, m_ImageMemory);
			m_Image = nullptr;
		}
	}

	EngineGraphicsFormats VulkanImage2D::GetImageFormat()
	{
		return m_Settings.format;
	}

	ImageLayout VulkanImage2D::GetLayout()
	{
		return  m_Settings.layout;
	}

	uint32_t VulkanImage2D::GetHeight()
	{
		return m_Settings.height;
	}

	uint32_t VulkanImage2D::GetWidth()
	{
		return m_Settings.width;
	}

	VulkanImage2D::~VulkanImage2D()
	{
		
		spdlog::info("Reloaded hahha");
	}
}
