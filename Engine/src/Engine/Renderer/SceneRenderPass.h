#pragma once
#include "Engine/Engine/Base.h"


namespace Polyboid
{
	class RenderResource;
	class GraphicsPipeline;
	class RenderPass;

	struct SceneRenderPassSettings
	{
		Ref<RenderPass> RenderPass;
		Ref<GraphicsPipeline> Pipeline;
	};

	using RenderResources = std::unordered_map<std::string, Ref<RenderResource>>;

	class SceneRenderPass
	{
	private:
		
		SceneRenderPassSettings m_Settings;
		//Textures or render pass
		// if render pass we need all the things


	public:

		SceneRenderPass(const SceneRenderPassSettings& settings);
		void SetInput(const std::string& inputName, Ref<RenderResource>& resource);
		static Ref<SceneRenderPass> Create(const SceneRenderPassSettings& settings);

	};

}

