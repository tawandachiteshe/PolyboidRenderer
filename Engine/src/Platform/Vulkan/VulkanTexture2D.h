#pragma once
#include "Engine/Renderer/Texture.h"
#include <vulkan/vulkan.hpp>
#include <vma/vk_mem_alloc.h>

namespace Polyboid
{
	class VulkanSamplerState;
	class VulkanImage2D;
	class VkRenderAPI;

	class VulkanTexture2D : public Texture
	{

	private:
		Ref<VulkanImage2D> m_Image = nullptr;
		vk::Image m_VulkanImage = nullptr;
		vk::ImageView m_View = nullptr;
		vk::Sampler m_ImageSampler = nullptr;
		const VkRenderAPI* m_Context = nullptr;
		VmaAllocation m_ImageMemory{};
		Ref<VulkanSamplerState> m_SamplerState = nullptr;
		VkDescriptorSet m_ImguiDS = nullptr;
		vk::DescriptorImageInfo m_ImageDescriptorInfo;

	public:
		VulkanTexture2D(const VkRenderAPI* context, const TextureSettings& settings, const void* data = nullptr);
		VulkanTexture2D(const VkRenderAPI* context, const std::any& imageHandle, const TextureSettings& settings);

		void Destroy();

		virtual vk::DescriptorImageInfo GetVulkanDescriptorImageInfo();

		void Bind(uint32_t slot) override;
		void UnBind() override;
		std::any GetViewHandle() override;
		std::any GetSamplerHandle() override;
		std::any GetImGuiTexture() override;
		
		void SetData(const void* data, uint32_t size) override;
		~VulkanTexture2D() override;
	};

	
}

