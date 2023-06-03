#pragma once
#include <vulkan/vulkan_raii.hpp>

#include "Engine/Renderer/Image2D.h"
#include "Engine/Renderer/Texture.h"
#include <vma/vk_mem_alloc.h>

namespace Polyboid
{
	class VkRenderAPI;

	class VulkanImage2D : public Image2D
	{
	private:
		vk::Image m_Image;
		uint32_t m_Width = 0, m_Height = 0;
		EngineGraphicsFormats m_Format = EngineGraphicsFormats::BGRA8;
		const VkRenderAPI* m_Context = nullptr;
		ImageSettings m_Settings;
		VmaAllocation m_ImageMemory;

	public:

		void Init(const VkRenderAPI* context, const ImageSettings& imageSettings);
		void Recreate();

		VulkanImage2D(const VkRenderAPI* context, const ImageSettings& imageSettings);
		std::any GetHandle() override;
		virtual void Destroy();
		EngineGraphicsFormats GetImageFormat() override;
		ImageLayout GetLayout() override;
		uint32_t GetHeight() override;
		uint32_t GetWidth() override;
		~VulkanImage2D() override;
	};

}
