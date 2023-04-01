#pragma once
#include "Engine/Renderer/BlendState.h"
#include <vulkan/vulkan.hpp>


namespace Polyboid
{
	class VkRenderAPI;
	class RenderAPI;

	class VulkanBlendState : public BlendState
	{

	private:
		bool m_Dirty = true;
		BlendMode m_BlendMode;
		std::vector<BlendMode> m_BlendModes;
		glm::vec4 m_ConstBlend;
		vk::PipelineColorBlendStateCreateInfo m_CreateInfo;


	public:
		VulkanBlendState();

		void SetBlendMode(const BlendMode& blendMode) override;
		void SetBlendModes(const std::vector<BlendMode>& blendModes) override;
		const std::vector<BlendMode>& GetBlendModes() const override;
		void SetSampleMask(uint32_t sampleMask) override;
		uint32_t GetSampleMask() const override;
		void SetAlphaCoverage(bool enabled) override;
		bool GetAlphaCoverage() const override;
		void SetIndependentBlend(bool enabled) override;
		bool GetIndependentBlend() const override;
		void SetConstBlendFactor(const glm::vec4& constantBlendFactor) override;
		const glm::vec4& GetConstBlendFactor() const override;
		~VulkanBlendState() override;

		virtual void Bind();
	};

}

