#include "boidpch.h"
#include "VulkanMultiSampleState.h"


namespace Polyboid
{
	void VulkanMultiSampleState::SetSampleMask(uint32_t sampleMask)
	{
		m_SampleMask = sampleMask;
	}

	uint32_t VulkanMultiSampleState::GetSampleMask() const
	{
		return  m_SampleMask;
	}

	void VulkanMultiSampleState::SetAlphaCoverage(bool enabled)
	{
		m_AlphaToCov = enabled;
	}

	bool VulkanMultiSampleState::GetAlphaCoverage() const
	{
		return m_AlphaToCov;
	}

	void VulkanMultiSampleState::SetSamplesCount(uint32_t count)
	{
		m_SamplesCount = count;
	}

	VulkanMultiSampleState::~VulkanMultiSampleState()
	{
	}

	vk::PipelineMultisampleStateCreateInfo VulkanMultiSampleState::GetVulkanInfo()
	{
		m_CreateInfo.alphaToCoverageEnable = m_AlphaToCov;
		m_CreateInfo.alphaToOneEnable = m_AlphaToOne;
		m_CreateInfo.pSampleMask = &m_SampleMask;
		m_CreateInfo.minSampleShading = 1.0f;
		m_CreateInfo.rasterizationSamples = vk::SampleCountFlagBits::e1;

		return m_CreateInfo;
	}
}
