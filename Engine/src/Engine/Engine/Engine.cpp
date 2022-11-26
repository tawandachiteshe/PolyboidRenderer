#include "boidpch.h"
#include "Engine.h"


namespace Polyboid
{
	Unique<EngineSettings> Engine::s_Data = std::make_unique<EngineSettings>();

	Ref<World> Engine::CreateWorld(std::string& name)
	{

		auto world = std::make_shared<World>(name);
		s_Data->m_Worlds.emplace_back(world);

		return world;
	}
}
