#include "boidpch.h"
#include "RenderPass.h"

#include "RenderAPI.h"


namespace Polyboid
{
	Ref<RenderPass> RenderPass::Create(const RenderPassSettings& settings)
	{
		return RenderAPI::Get()->CreateRenderPass(settings);
	}
}
