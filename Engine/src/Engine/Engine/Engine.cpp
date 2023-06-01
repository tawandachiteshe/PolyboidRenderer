#include "boidpch.h"
#include "Engine.h"

#include "Engine/Renderer/Framebuffer.h"
#include "Scripting/ScriptingEngine.h"
#include "nlohmann/json.hpp"


namespace Polyboid
{
	Unique<EngineSettings> Engine::s_Data = std::make_unique<EngineSettings>();


	void Engine::Init()
	{
		ScriptingEngine::Init();
		

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
