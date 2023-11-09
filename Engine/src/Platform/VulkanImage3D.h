#pragma once
#include "Engine/Renderer/Image2D.h"
#include "Vulkan/Utils/VulkanAllocator.h"


namespace Polyboid
{
	class VulkanImage3D : public Image3D
	{
	private:
		vk::Image m_Image;
		uint32_t m_Width = 0, m_Height = 0, m_Depth = 0;
		EngineGraphicsFormats m_Format = EngineGraphicsFormats::BGRA8;
		ImageSettings m_Settings;
		VmaAllocation m_ImageMemory;
	public:
		explicit VulkanImage3D(const ImageSettings& imageSettings);

		void Destroy() override;
		void Recreate() override;
		std::any GetHandle() override;
		EngineGraphicsFormats GetImageFormat() override;
		ImageLayout GetLayout() override;
		uint32_t GetHeight() override;
		uint32_t GetWidth() override;
		uint32_t GetDepth() override;
	};

}

