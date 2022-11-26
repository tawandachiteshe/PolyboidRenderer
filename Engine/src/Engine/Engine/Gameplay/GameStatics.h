#pragma once
#include "Engine/Engine/Base.h"
#include "Engine/Renderer/Camera.h"

//temp thing here idk wtf im doing

namespace Polyboid
{
	class World;

	struct GameInstanceData
	{
		Ref<Camera> m_CurrentCamera;
		Ref<World> m_CurrentWorld = nullptr;
	};

	class GameStatics
	{
	public:
		static Ref<GameInstanceData> m_sData;
		static void SetCurrentCamera(const Ref<Camera>& camera);

		static void SetCurrentWorld(const Ref<World>& world);

		static Ref<World>& GetCurrentWorld();

		static Ref<Camera>& GetCurrentCamera();

	};
	
}
