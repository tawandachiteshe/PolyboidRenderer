#include "boidpch.h"
#include "VulkanSamplerState.h"

#include "VkRenderAPI.h"
#include "Utils/VulkanDevice.h"


namespace Polyboid
{

	vk::SamplerAddressMode ToVkSamplerMode(TextureWrap wrap)
	{
		switch (wrap)
		{
		case TextureWrap::Repeat: return vk::SamplerAddressMode::eRepeat;
		case TextureWrap::ClampToEdge: return vk::SamplerAddressMode::eClampToEdge;
		case TextureWrap::MirroredRepeat: return vk::SamplerAddressMode::eMirroredRepeat;
		case TextureWrap::MirrorClampToEdge: return vk::SamplerAddressMode::eMirrorClampToEdge;
		case TextureWrap::ClampToBorder: return vk::SamplerAddressMode::eClampToBorder;
		}
	}

	vk::Filter toMinFilter(MinFilterMode filter)
	{
		switch (filter)
		{
		case MinFilterMode::Nearest: return vk::Filter::eNearest;
		case MinFilterMode::Linear: return vk::Filter::eLinear;
		case MinFilterMode::MipNearest: 
		case MinFilterMode::MipLinear: 
		case MinFilterMode::Anisotropic:
			__debugbreak();
		}
	}

	vk::SamplerMipmapMode toMipMapMode(MinFilterMode filter)
	{
		switch (filter)
		{
		case MinFilterMode::Nearest: 
		case MinFilterMode::Linear:
			__debugbreak();
		case MinFilterMode::MipNearest: return vk::SamplerMipmapMode::eNearest;
		case MinFilterMode::MipLinear: return vk::SamplerMipmapMode::eLinear;
		case MinFilterMode::Anisotropic:
			__debugbreak();
		}
	}

	vk::Filter toMagFilter(MagFilterMode filter)
	{
		switch (filter)
		{
		case MagFilterMode::Nearest: return vk::Filter::eNearest;
		case MagFilterMode::Linear: return vk::Filter::eLinear;
		}
	}


	VulkanSamplerState::VulkanSamplerState(const VkRenderAPI* context, const SamplerSettings& samplerSettings)
		: m_Context(context), m_Settings(samplerSettings)
	{

		auto device = context->GetDevice()->GetVulkanDevice();

		vk::SamplerCreateInfo createInfo;
		createInfo.borderColor = vk::BorderColor::eFloatOpaqueBlack;
		createInfo.addressModeU = ToVkSamplerMode(m_Settings.wrap);
		createInfo.addressModeV = ToVkSamplerMode(m_Settings.wrap);
		createInfo.addressModeW = ToVkSamplerMode(m_Settings.wrap);
		createInfo.minFilter = toMinFilter(m_Settings.minFilter);
		createInfo.magFilter = toMagFilter(m_Settings.magFilter);
		createInfo.mipmapMode = toMipMapMode(m_Settings.minFilter);

		createInfo.anisotropyEnable = m_Settings.minFilter == MinFilterMode::Anisotropic;
		createInfo.maxAnisotropy = 8.0f;

		//I will need this when im doing pcf

		createInfo.compareOp = vk::CompareOp::eAlways;
		createInfo.compareEnable = false;
		createInfo.mipmapMode = vk::SamplerMipmapMode::eLinear;
		createInfo.minLod = static_cast<float>(m_Settings.minLod);
		createInfo.maxLod = static_cast<float>(m_Settings.magLod);
		createInfo.unnormalizedCoordinates = false;
		createInfo.mipLodBias = 0.0f;

		auto [result, sampler] = device.createSampler(createInfo);

		vk::resultCheck(result, "Failed to create Sampler");

		m_Handle = sampler;

	}

	void VulkanSamplerState::SetFilterMode(const std::pair<MinFilterMode, MagFilterMode>& filter)
	{
		const auto [min, mag] = filter;
		m_MinMode = min;
		m_MagMode = mag;
	}

	std::pair<MinFilterMode, MagFilterMode> VulkanSamplerState::GetFilterMode()
	{
		return {m_MinMode, m_MagMode};
	}

	void VulkanSamplerState::SetWrapMode(const TextureWrap& wrap)
	{
		m_Wrap = wrap;
	}

	TextureWrap VulkanSamplerState::GetWrapMode()
	{
		return m_Wrap;
	}

	void VulkanSamplerState::SetCompareFunc(const CompareFunction& wrap)
	{
		m_ComFunc = wrap;
	}

	CompareFunction VulkanSamplerState::GetCompareFunc()
	{
		return m_ComFunc;
	}

	void VulkanSamplerState::SetBorderColor(const glm::vec4& borderColor)
	{
		m_BorderColor = borderColor;
	}

	glm::vec4 VulkanSamplerState::GetBorderColor() const
	{
		return m_BorderColor;
	}



	void VulkanSamplerState::Bind(uint32_t slot)
	{
		if (m_Dirty)
		{
		
			m_Dirty = false;
		}
	}

	void VulkanSamplerState::UnBind()
	{
	}

	VulkanSamplerState::~VulkanSamplerState()
	{
	}
}
