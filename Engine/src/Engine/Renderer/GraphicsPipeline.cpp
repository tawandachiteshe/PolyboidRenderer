#include "boidpch.h"
#include "GraphicsPipeline.h"

#include "RenderAPI.h"


namespace Polyboid
{

	Ref<GraphicsPipeline> GraphicsPipeline::CreateGraphicsPipeline()
	{
		return RenderAPI::Get()->CreatePipelineState();
	}
}
