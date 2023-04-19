#include "boidpch.h"
#include "VulkanTexture2D.h"
#include <vulkan/vulkan.hpp>
#include "Utils/VulkanDevice.h"
#include "Utils/VulkanAllocator.h"
#include <vma/vk_mem_alloc.h>

#include "VkRenderAPI.h"
#include "Utils/Common.h"
#include "stb/stb_image.h"


namespace Polyboid
{
	VulkanTexture2D::VulkanTexture2D(const VkRenderAPI* context, const void* data, const TextureSettings& settings): m_Context(context)
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
		case TextureUsage::Attachment:createInfo.usage = eColorAttachment | eDepthStencilAttachment; break;
		case TextureUsage::Sampling: createInfo.usage = eSampled | eTransferDst; break;
		}

		createInfo.sharingMode = vk::SharingMode::eExclusive;
		createInfo.samples = vk::SampleCountFlagBits::e1;

		vk::ImageCreateInfo::NativeType vulkanImageInfo = createInfo;

		VmaAllocationCreateInfo allocInfo = {};
		allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
		allocInfo.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
		allocInfo.priority = 1.0f;
		VmaAllocation allocation;


		vk::Image::NativeType vulkanImage;
		vmaCreateImage(allocator, &vulkanImageInfo, &allocInfo, &vulkanImage, &allocation, nullptr);
		m_Image = vulkanImage;
		m_ImageMemory = allocation;

		if (!settings.path.empty())
		{

			std::string imagePath = settings.path;
			void* allocData = nullptr;
			vmaMapMemory(allocator, allocation, &allocData);

			int32_t w = 0, h = 0, c = 0;
			uint8_t* pixels = stbi_load(imagePath.data(), &w, &h, &c, 0);

			std::memcpy(allocData, pixels, w * h * c);
			stbi_image_free(pixels);
			vmaUnmapMemory(allocator, allocation);

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
		createViewInfo.image = vulkanImage;
		createViewInfo.components = vk::ComponentMapping();
		createViewInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		createViewInfo.subresourceRange.baseMipLevel = 0;
		createViewInfo.subresourceRange.levelCount = 1;
		createViewInfo.subresourceRange.baseArrayLayer = 0;
		createViewInfo.subresourceRange.layerCount = 1;

		auto [createViewResult, view] = device.createImageView(createViewInfo);
		vk::resultCheck(createViewResult, "Failed to create image view");

		m_View = view;


	}

	VulkanTexture2D::VulkanTexture2D(const VkRenderAPI* context, const TextureSettings& settings,
		const std::any& imageHandle): m_Context(context), m_ImageMemory(nullptr)
	{
		vk::Device device = (*context->GetDevice());
		vk::Image image = std::any_cast<vk::Image>(imageHandle);
		m_Image = image;


		vk::ImageViewCreateInfo createViewInfo;
		createViewInfo.flags = vk::ImageViewCreateFlags();
		createViewInfo.sType = vk::StructureType::eImageViewCreateInfo;
		createViewInfo.viewType = vk::ImageViewType::e2D;
		createViewInfo.format = Utils::ConvertToVulkanFormat(settings.sizedFormat);
		createViewInfo.image = image;
		createViewInfo.components = vk::ComponentMapping();
		createViewInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		createViewInfo.subresourceRange.baseMipLevel = 0;
		createViewInfo.subresourceRange.levelCount = 1;
		createViewInfo.subresourceRange.baseArrayLayer = 0;
		createViewInfo.subresourceRange.layerCount = 1;

		auto [createViewResult, view] = device.createImageView(createViewInfo);
		vk::resultCheck(createViewResult, "Failed to create image view");

		m_View = view;
	}

	void VulkanTexture2D::Destroy()
	{

		vk::Device device = (*m_Context->GetDevice());
		if (m_View != vk::ImageView(nullptr))
		{
			device.destroyImageView(m_View);
			m_View = nullptr;
		}

		if (m_Image != vk::Image(nullptr) && m_ImageMemory != nullptr)
		{
			vk::Device device = (*m_Context->GetDevice());
			VmaAllocator allocator = (*m_Context->GetAllocator());

			vmaDestroyImage(allocator, m_Image, m_ImageMemory);
			m_Image = nullptr;
		}
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
