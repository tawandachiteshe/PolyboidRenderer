#pragma once
#include "Engine/Renderer/Texture2D.h"


namespace Polyboid
{
	class VulkanImage2D;
	class VulkanSamplerState;
	class VulkanImage3D;

	class VulkanTexture3D : public Texture3D
	{
	private:
		TextureSettings m_TextureSettings;
		Ref<VulkanImage2D> m_Image;
		Ref<VulkanSamplerState> m_SamplerState;
		vk::DescriptorImageInfo m_ImageDescriptorInfo;
		vk::ImageView m_View;
	public:
		VulkanTexture3D(const void* data, const TextureSettings& textureSettings);

		void Destroy() override;
		void Recreate() override;

		void Bind(uint32_t slot) override;
		void UnBind() override;
		void SetData(const void** data, uint32_t size) override;
		std::any GetViewHandle() override;
		std::any GetSamplerHandle() override;
		vk::DescriptorImageInfo GetVulkanDescriptorImageInfo();
		TextureType GetTextureType() override;

		RenderResourceType GetRenderResourceType() override;
	};

}

