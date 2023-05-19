#include "boidpch.h"
#include "PipelineState.h"

#include "RenderAPI.h"


namespace Polyboid
{
	Ref<PipelineState> PipelineState::CreateGraphicsPipeline()
	{
		return RenderAPI::Get()->CreatePipelineState();
	}
}
