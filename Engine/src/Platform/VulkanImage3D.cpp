#include "boidpch.h"
#include "VulkanImage3D.h"

#include "Vulkan/VkRenderAPI.h"
#include "Vulkan/Utils/Common.h"


namespace Polyboid
{
	VulkanImage3D::VulkanImage3D(const ImageSettings& imageSettings): m_Settings(imageSettings)
	{

		VmaAllocator allocator = VkRenderAPI::GetVulkanAllocator ();

		vk::ImageCreateInfo createInfo{};
		createInfo.flags = vk::ImageCreateFlags();
		createInfo.sType = vk::StructureType::eImageCreateInfo;
		createInfo.imageType = vk::ImageType::e3D;
		createInfo.extent.width = imageSettings.width;
		createInfo.extent.height = imageSettings.height;
		createInfo.extent.depth = imageSettings.depth;

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
		case ImageUsage::Sampling: createInfo.usage = imageSettings.generateMips ? eTransferSrc | eSampled | eTransferDst : eSampled | eTransferDst; break;
		case ImageUsage::ColorAttachment: createInfo.usage = eColorAttachment; break;
		case ImageUsage::DepthStencilAttachment: createInfo.usage = eDepthStencilAttachment; break;
		case ImageUsage::TransferSrc: createInfo.usage = eTransferSrc; break;
		case ImageUsage::TransferDst: createInfo.usage = eTransferDst; break;
		case ImageUsage::ColorAttachmentSampling: createInfo.usage = imageSettings.generateMips ? eTransferSrc | eColorAttachment | eSampled : eColorAttachment | eSampled;  break;
		case ImageUsage::DepthStencilAttachmentSampling: createInfo.usage = eDepthStencilAttachment | eSampled;  break;
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
		auto result = vmaCreateImage(allocator, &vulkanImageInfo, &allocInfo, &vulkanImage, &allocation, nullptr);

		if (result != VK_SUCCESS)
		{
			__debugbreak();
		}

		m_Image = vulkanImage;
		m_ImageMemory = allocation;

	}

	void VulkanImage3D::Destroy()
	{
	}

	void VulkanImage3D::Recreate()
	{
	}

	std::any VulkanImage3D::GetHandle()
	{
		return m_Image;
	}

	EngineGraphicsFormats VulkanImage3D::GetImageFormat()
	{
		return m_Settings.format;
	}

	ImageLayout VulkanImage3D::GetLayout()
	{
		return m_Settings.layout;
	}

	uint32_t VulkanImage3D::GetHeight()
	{
		return m_Settings.height;
	}

	uint32_t VulkanImage3D::GetWidth()
	{
		return m_Settings.width;
	}

	uint32_t VulkanImage3D::GetDepth()
	{
		return m_Settings.depth;
	}
}
