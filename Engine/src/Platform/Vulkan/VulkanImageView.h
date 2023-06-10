#pragma once
#include "Engine/Engine/Base.h"
#include "Engine/Renderer/Image2D.h"
#include "Engine/Renderer/ImageView.h"


namespace Polyboid
{
	class Image2D;

	class VulkanImageView2D : public ImageView2D
	{
	private:
		vk::ImageView m_Handle;
	public:

		VulkanImageView2D(const Ref<Image2D>& image, const ImageViewSettings& imageSettings);
		std::any GetHandle() override;
	};
}


