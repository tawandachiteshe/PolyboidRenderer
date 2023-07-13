#include "boidpch.h"
#include "SceneRenderPass.h"



namespace Polyboid
{
	SceneRenderPass::SceneRenderPass(const SceneRenderPassSettings& settings): m_Settings(settings)
	{
	}

	Ref<SceneRenderPass> SceneRenderPass::Create(const SceneRenderPassSettings& settings)
	{
		return CreateRef<SceneRenderPass>(settings);
	}
}
