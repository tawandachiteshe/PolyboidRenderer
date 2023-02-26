#include "boidpch.h"
#include "Engine.h"

#include "Serializer.h"
#include "Debug/Timer.h"
#include "Engine/Renderer/Framebuffer.h"
#include "Engine/Renderer/Renderer.h"
#include "Scripting/ScriptingEngine.h"
#include "Shaders/ShaderCompiler.h"
#include "nlohmann/json.hpp"
#include "taskflow/taskflow.hpp"
#include "hashpp/hashpp.h"

#include <pods/pods.h>
#include <pods/msgpack.h>
#include <pods/streams.h>

#include "Engine/Renderer/Renderer2D.h"
#include "Registry/ShaderRegistry.h"

namespace Polyboid
{
	Unique<EngineSettings> Engine::s_Data = std::make_unique<EngineSettings>();


	void Engine::Init()
	{
		ScriptingEngine::Init();
		

	}

	void Engine::InitRenderer(const Ref<RenderAPI>& context)
	{
		ShaderRegistry::Init();

		Renderer::Init(context);
		Renderer2D::Init(context);

	}


	void Engine::CreateWorld(const std::string& name)
	{
	}

	void Engine::SaveWorld(const std::string& path)
	{
	}

	Ref<World> Engine::LoadWorld(const std::string& path)
	{
		return nullptr;
	}
}
