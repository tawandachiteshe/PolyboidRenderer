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
	VulkanTexture2D::VulkanTexture2D(const VkRenderAPI* context, const TextureSettings& settings)
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

		// char* stats = nullptr;
		// vmaBuildStatsString(allocator, &stats, true);
		//
		// vmaFreeStatsString(allocator, stats);

		vmaDestroyImage(allocator, vulkanImage, allocation);

	}

	void VulkanTexture2D::Destroy()
	{
		
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
