#pragma once
#include "World/World.h"


namespace Polyboid
{
	class RenderAPI;
	class RenderTarget;

	struct EngineSettings
	{
		std::vector<World> m_Worlds{};
	};


	//For mananaging system and settings
	class Engine
	{
	public:

		static void Init();
		static void InitRenderer(const Ref<RenderAPI>& context);
		static void CreateWorld(const std::string& name);

		static void SaveWorld(const std::string& path);
		static Ref<World> LoadWorld(const std::string& path);
		

		static  Unique<EngineSettings> s_Data;

	public:
		
	};
	
}
