#pragma once
#include "Engine/Renderer/Texture.h"
#include <vulkan/vulkan.hpp>

#include "Utils/VulkanAllocator.h"

namespace Polyboid
{
	class VkRenderAPI;

	class VulkanTexture2D : public Texture
	{

	private:
		vk::Image m_Image = nullptr;
		vk::ImageView m_View = nullptr;
		const VkRenderAPI* m_Context = nullptr;
		TextureSettings m_Settings;
		VmaAllocation m_Allocation;

	public:
		VulkanTexture2D(const VkRenderAPI* context, const TextureSettings& settings);
		VulkanTexture2D(const std::any& handle);

		virtual  void Destroy(VkRenderAPI* context = nullptr);

		void Bind(uint32_t slot) override;
		void UnBind() override;
		std::any GetHandle() override;
		void SetData(const void* data, uint32_t size) override;
		~VulkanTexture2D() override;
	};

	
}

