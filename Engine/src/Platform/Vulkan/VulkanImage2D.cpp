#include "boidpch.h"
#include "VulkanImage2D.h"

#include <vulkan/vulkan.hpp>
#include "Utils/VulkanAllocatorInstance.h"
#include "vma/vk_mem_alloc.h"
#include "Utils/VulkanDevice.h"
#include "VkRenderAPI.h"
#include "Utils/Common.h"


namespace Polyboid
{
	VulkanImage2D::VulkanImage2D(const VkRenderAPI* context, const ImageSettings& imageSettings): m_Context(context), m_Settings(imageSettings)
	{

		vk::Device device = (*context->GetDevice());
		VmaAllocator allocator = (*context->GetAllocator());

		vk::ImageCreateInfo createInfo{};
		createInfo.flags = vk::ImageCreateFlags();
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

		createInfo.arrayLayers = 1;
		createInfo.format = Utils::ConvertToVulkanFormat(imageSettings.format);
		createInfo.tiling = vk::ImageTiling::eOptimal;

		createInfo.initialLayout = Utils::ConvertToVulkanLayout(imageSettings.layout);

		using enum vk::ImageUsageFlagBits;

		switch (imageSettings.usage)
		{
		case ImageUsage::Swapchain: break;
		case ImageUsage::ColorDepthStencilAttachment:createInfo.usage = eColorAttachment | eDepthStencilAttachment; break;
		case ImageUsage::Sampling: createInfo.usage = eSampled | eTransferDst; break;
		case ImageUsage::ColorAttachment: createInfo.usage = eColorAttachment; break;
		case ImageUsage::DepthStencilAttachment: createInfo.usage = eDepthStencilAttachment; break;
		case ImageUsage::TransferSrc: createInfo.usage = eTransferSrc; break;
		case ImageUsage::TransferDst: createInfo.usage = eTransferDst; break;
		case ImageUsage::ColorAttachmentSampling: createInfo.usage = eColorAttachment | eSampled;  break;
		case ImageUsage::DepthStencilAttachmentSampling: createInfo.usage = eDepthStencilAttachment | eSampled;  break;
		default: ;
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
		vmaCreateImage(allocator, &vulkanImageInfo, &allocInfo, &vulkanImage, &allocation, nullptr);
		m_Image = vulkanImage;
		m_ImageMemory = allocation;

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
}
