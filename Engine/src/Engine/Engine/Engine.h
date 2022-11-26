#pragma once
#include "Gameplay/World.h"


namespace Polyboid
{

	//Change Setttings n shit here

	struct EngineSettings
	{
		std::vector<Ref<World>> m_Worlds;
	};

	class Engine
	{
	private:

		static  Unique<EngineSettings> s_Data;

	public:

		static  Ref<World> CreateWorld(std::string& name);
		
	};
	
}
