#include "boidpch.h"
#include "VulkanImageView.h"

#include "Engine/Renderer/Image2D.h"
#include "VkRenderAPI.h"
#include "Utils/Common.h"


namespace Polyboid
{
	VulkanImageView2D::VulkanImageView2D(const Ref<Image2D>& image, const ImageViewSettings& imageSettings)
	{

		vk::ImageViewCreateInfo createViewInfo;
		createViewInfo.flags = vk::ImageViewCreateFlags();
		createViewInfo.sType = vk::StructureType::eImageViewCreateInfo;
		createViewInfo.viewType = vk::ImageViewType::e2D;
		createViewInfo.format = Utils::ConvertToVulkanFormat(image->GetImageFormat());
		createViewInfo.image = std::any_cast<vk::Image>(image->GetHandle());
		createViewInfo.components = vk::ComponentMapping();
		createViewInfo.subresourceRange.aspectMask = Utils::ImageFormatToAspectBits(image->GetImageFormat()); //vk::ImageAspectFlagBits::eColor;
		createViewInfo.subresourceRange.baseMipLevel = 0;
		createViewInfo.subresourceRange.levelCount = imageSettings.MipLevels;
		createViewInfo.subresourceRange.baseArrayLayer = 0;
		createViewInfo.subresourceRange.layerCount = imageSettings.LayerCount;

		auto [createViewResult, view] = VkRenderAPI::GetVulkanDevice().createImageView(createViewInfo);
		vk::resultCheck(createViewResult, "Failed to create image view");
		m_Handle = view;
	}

	std::any VulkanImageView2D::GetHandle()
	{
		return m_Handle;
	}
}
