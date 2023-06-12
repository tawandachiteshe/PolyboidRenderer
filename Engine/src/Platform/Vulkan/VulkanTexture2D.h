#pragma once
#include "Engine/Renderer/Texture2D.h"
#include <vulkan/vulkan_raii.hpp>
#include <vma/vk_mem_alloc.h>

namespace Polyboid
{
	class VulkanSamplerState;
	class VulkanImage2D;
	class VkRenderAPI;
	class Image2D;

	class VulkanTexture2D : public Texture2D
	{

	private:
		Ref<VulkanImage2D> m_Image = nullptr;
		vk::ImageView m_View = nullptr;
		vk::Sampler m_ImageSampler = nullptr;
		const VkRenderAPI* m_Context = nullptr;
		VmaAllocation m_ImageMemory{};
		Ref<VulkanSamplerState> m_SamplerState = nullptr;
		VkDescriptorSet m_ImguiDS = nullptr;
		vk::DescriptorImageInfo m_ImageDescriptorInfo;
		const void* m_Data = nullptr;
		TextureSettings m_Settings;
		vk::Image m_VulkanImage;

	public:

		void Init(const VkRenderAPI* context, const TextureSettings& settings, const void* data = nullptr);
		void Recreate() override;
		void Destroy() override;

		VulkanTexture2D(const VkRenderAPI* context, const TextureSettings& settings, const void* data = nullptr);
		VulkanTexture2D(const VkRenderAPI* context, const vk::Image& imageHandle);
		explicit VulkanTexture2D(const Ref<Image2D>& image);

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

