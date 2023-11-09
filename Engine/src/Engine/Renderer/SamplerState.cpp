#include "boidpch.h"
#include "SamplerState.h"

#include "RenderAPI.h"

namespace Polyboid
{
	

	Ref<SamplerState> SamplerState::Create(const SamplerSettings& settings)
	{
		return RenderAPI::Get()->CreateSampler(settings);
	}
}
