#include "boidpch.h"
#include "VulkanTexture2D.h"
#include <vulkan/vulkan.hpp>
#include "Utils/VulkanDevice.h"
#include "Utils/VulkanAllocator.h"
#include <vma/vk_mem_alloc.h>

#include "VkRenderAPI.h"
#include "Utils/Common.h"


namespace Polyboid
{
	VulkanTexture2D::VulkanTexture2D(const VkRenderAPI* context, const TextureSettings& settings): m_Context(context), m_Settings(settings)
	{

		vk::Device device = (*context->GetDevice());
		VmaAllocator allocator = (*context->GetAllocator());

		vk::ImageCreateInfo createInfo{};
		createInfo.flags = vk::ImageCreateFlags();
		createInfo.sType = vk::StructureType::eImageCreateInfo;
		createInfo.imageType = vk::ImageType::e2D;
		createInfo.extent.width = settings.Width;
		createInfo.extent.height = settings.Height;
		createInfo.extent.depth = 1;

		if (settings.generateMips)
		{
			createInfo.mipLevels = settings.mipCount;
		}
		else
		{
			createInfo.mipLevels = 1;
		}

		createInfo.arrayLayers = 1;
		createInfo.format = Utils::ConvertToVulkanFormat(settings.sizedFormat);
		createInfo.tiling = vk::ImageTiling::eOptimal;

		createInfo.initialLayout = vk::ImageLayout::eUndefined;

		using enum vk::ImageUsageFlagBits;

		switch (settings.usage)
		{
		case TextureUsage::Swapchain: break;
		case TextureUsage::DepthAttachment:createInfo.usage = eDepthStencilAttachment; break;
		case TextureUsage::ColorAttachment: createInfo.usage = eColorAttachment; break;
		case TextureUsage::Sampling: createInfo.usage = eSampled; break;
		}

		createInfo.sharingMode = vk::SharingMode::eExclusive;
		createInfo.samples = vk::SampleCountFlagBits::e1;

		vk::ImageCreateInfo::NativeType vulkanImageInfo = createInfo;

		VmaAllocationCreateInfo allocInfo = {};
		allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
		allocInfo.priority = 1.0f;
		VmaAllocation allocation;


		vk::Image::NativeType vulkanImage;
		vmaCreateImage(allocator, &vulkanImageInfo, &allocInfo, &vulkanImage, &allocation, nullptr);

		m_Allocation = allocation;

		m_Image = vulkanImage;

		vk::ImageViewCreateInfo imageViewCreateInfo{};
		imageViewCreateInfo.image = m_Image;
		imageViewCreateInfo.format = createInfo.format;
		imageViewCreateInfo.components.r = vk::ComponentSwizzle::eIdentity;
		imageViewCreateInfo.components.g = vk::ComponentSwizzle::eIdentity;
		imageViewCreateInfo.components.b = vk::ComponentSwizzle::eIdentity;
		imageViewCreateInfo.components.a = vk::ComponentSwizzle::eIdentity;
		imageViewCreateInfo.viewType = vk::ImageViewType::e2D;
		switch (createInfo.format)
		{
		case vk::Format::eD24UnormS8Uint:
		case vk::Format::eD16UnormS8Uint:
		case vk::Format::eD32SfloatS8Uint:
			imageViewCreateInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil;
			break;
		case vk::Format::eD16Unorm:
		case vk::Format::eD32Sfloat:
			imageViewCreateInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth;
			break;
			default:
				imageViewCreateInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		}
		
		imageViewCreateInfo.subresourceRange.layerCount = 1;
		imageViewCreateInfo.subresourceRange.levelCount = 1;
		imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
		imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
		auto[res, view] = device.createImageView(imageViewCreateInfo);

		vk::resultCheck(res, "Failed to create image view");

		m_View = view;


		// char* stats = nullptr;
		// vmaBuildStatsString(allocator, &stats, true);
		//
		// vmaFreeStatsString(allocator, stats);

		

	}

	void VulkanTexture2D::Destroy(VkRenderAPI* context)
	{
		if (context)
		{

			vk::Device device = (*context->GetDevice());
			VmaAllocator allocator = (*context->GetAllocator());

			if (m_Image != vk::Image(nullptr))
			{

				vmaDestroyImage(allocator, m_Image, m_Allocation);
			}

			if (m_View != vk::ImageView(nullptr))
			{
				device.destroyImageView(m_View);
			}

		}
		else
		{

			vk::Device device = (*m_Context->GetDevice());
			VmaAllocator allocator = (*m_Context->GetAllocator());

			if (m_View != vk::ImageView(nullptr))
			{
				device.destroyImageView(m_View);
			}


			if (m_Image != vk::Image(nullptr))
			{
				
				vmaDestroyImage(allocator, m_Image, m_Allocation);
			}

		
		}
	
	}

	VulkanTexture2D::VulkanTexture2D(const std::any& handle)
	{
		m_View = std::any_cast<vk::ImageView>(handle);
	}

	void VulkanTexture2D::Bind(uint32_t slot)
	{
	}

	void VulkanTexture2D::UnBind()
	{
	}

	std::any VulkanTexture2D::GetHandle()
	{
		return m_View;
	}

	void VulkanTexture2D::SetData(const void* data, uint32_t size)
	{
	}

	VulkanTexture2D::~VulkanTexture2D()
	{
	}
}
