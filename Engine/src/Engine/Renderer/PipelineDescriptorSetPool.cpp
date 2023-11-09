#include "boidpch.h"
#include "PipelineDescriptorSetPool.h"

#include "RenderAPI.h"


namespace Polyboid
{
	Ref<PipelineDescriptorSetPool> PipelineDescriptorSetPool::Create(const DescriptorSetPoolSettings& settings)
	{
		return RenderAPI::Get()->CreateDescriptorSetPool(settings);
	}
}
