#pragma once
#include "Engine/Renderer/MultiSampleState.h"
#include <vulkan/vulkan.hpp>

namespace Polyboid
{
	class VulkanMultiSampleState : public MultiSampleState
	{
	private:
		vk::PipelineMultisampleStateCreateInfo m_CreateInfo{};

		uint32_t m_SampleMask = 0xFF'FF'FF'FF;
		uint32_t m_SamplesCount = 0;

		bool m_AlphaToCov = false;
		bool m_AlphaToOne = false;


	public:
		void SetSampleMask(uint32_t sampleMask) override;
		uint32_t GetSampleMask() const override;
		void SetAlphaCoverage(bool enabled) override;
		bool GetAlphaCoverage() const override;
		void SetSamplesCount(uint32_t count) override;
		~VulkanMultiSampleState() override;

		virtual vk::PipelineMultisampleStateCreateInfo GetVulkanInfo();
	};
}


