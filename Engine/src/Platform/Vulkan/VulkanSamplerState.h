#pragma once
#include "Engine/Renderer/SamplerState.h"
#include <vulkan/vulkan.hpp>

namespace Polyboid
{
	class VkRenderAPI;

	class VulkanSamplerState : public SamplerState
	{

		const VkRenderAPI* m_Context;
		SamplerSettings m_Settings;
		MinFilterMode m_MinMode = MinFilterMode::Linear;
		MagFilterMode m_MagMode = MagFilterMode::Linear;
		TextureWrap m_Wrap = TextureWrap::ClampToEdge;
		glm::vec4 m_BorderColor{0.0};
		CompareFunction m_ComFunc = CompareFunction::Always;

		bool m_Dirty = true;
		vk::Sampler m_Handle;

	public:

		VulkanSamplerState(const VkRenderAPI* context, const SamplerSettings& samplerSettings);
		void SetFilterMode(const std::pair<MinFilterMode, MagFilterMode>& filter) override;
		std::pair<MinFilterMode, MagFilterMode> GetFilterMode() override;
		void SetWrapMode(const TextureWrap& wrap) override;
		TextureWrap GetWrapMode() override;
		void SetCompareFunc(const CompareFunction& wrap) override;
		CompareFunction GetCompareFunc() override;
		void SetBorderColor(const glm::vec4& borderColor) override;
		glm::vec4 GetBorderColor() const override;
		void Bind(uint32_t slot) override;
		void UnBind() override;
		~VulkanSamplerState() override;
	};

	
}

