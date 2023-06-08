#include "boidpch.h"
#include "GraphicsPipeline.h"

#include "RenderAPI.h"


namespace Polyboid
{

	Ref<GraphicsPipeline> GraphicsPipeline::Create()
	{
		return RenderAPI::Get()->CreatePipelineState();
	}
}
