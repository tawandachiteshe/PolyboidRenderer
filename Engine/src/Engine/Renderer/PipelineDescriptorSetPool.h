#pragma once
#include <any>

#include "Engine/Engine/Base.h"

namespace Polyboid
{
	class GraphicsPipeline;
	class PipelineDescriptorSet;

	struct DescriptorSetPoolSettings
	{
		uint32_t ImageCount = 3;
		uint32_t CombinedSamplerCount = ImageCount;
		uint32_t UniformBufferCount = ImageCount;
		uint32_t StorageBufferCount = ImageCount;
		uint32_t DescriptorCount = 1000;
		DescriptorSetPoolSettings() = default;


		DescriptorSetPoolSettings(uint32_t imageCount, uint32_t descriptorCount = 1000): ImageCount(imageCount), DescriptorCount(descriptorCount)
		{

			if (descriptorCount == 0)
			{
				__debugbreak();
			}

			CombinedSamplerCount = ImageCount * descriptorCount;
			UniformBufferCount = ImageCount * descriptorCount;
			StorageBufferCount = ImageCount * descriptorCount;

		}

	};

	class PipelineDescriptorSetPool
	{
	public:
		virtual Ref<PipelineDescriptorSet> GetDescriptorSet(uint32_t index = 0) = 0;
		virtual void Reset() = 0;
		virtual std::any GetHandle() = 0;
		virtual ~PipelineDescriptorSetPool() = default;
		virtual DescriptorSetPoolSettings GetDescPoolSettings() = 0;
		static Ref<PipelineDescriptorSetPool> Create(const DescriptorSetPoolSettings& settings);
	};
}
