#pragma once
#include "Engine/Renderer/Texture.h"
#include <vulkan/vulkan.hpp>

namespace Polyboid
{
	class VkRenderAPI;

	class VulkanTexture2D : public Texture
	{

	private:
		vk::Image m_Image;
		vk::ImageView m_View;
		const VkRenderAPI* m_Context = nullptr;

	public:
		VulkanTexture2D(const VkRenderAPI* context, const TextureSettings& settings);
		VulkanTexture2D(const VkRenderAPI* context, const std::any& handle);

		void Destroy();

		void Bind(uint32_t slot) override;
		void UnBind() override;
		std::any GetHandle() override;
		void SetData(const void* data, uint32_t size) override;
		~VulkanTexture2D() override;
	};

	
}

