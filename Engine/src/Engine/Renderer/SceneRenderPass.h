#pragma once
#include "Engine/Engine/Base.h"


namespace Polyboid
{
	class GraphicsPipeline;
	class RenderPass;

	class SceneRenderPass
	{
	private:

		Ref<RenderPass> m_RenderPass;
		Ref<GraphicsPipeline> m_Pipeline;
		//Textures or render pass
		// if render pass we need all the things


	public:

		SceneRenderPass(const Ref<RenderPass>& renderPass, const Ref<GraphicsPipeline>& graphicsPipeline);

	};

}

