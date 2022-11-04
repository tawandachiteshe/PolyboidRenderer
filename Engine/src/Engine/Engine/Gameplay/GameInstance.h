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
		World* m_CurrentWorld = nullptr;
	};

	class GameInstance
	{
	public:
		static Ref<GameInstanceData> m_sData;
		static void SetCurrentCamera(const Ref<Camera>& camera);

		static void SetCurrentWorld(World* world);

		static World* GetCurrentWorld();


		static Ref<Camera>& GetCurrentCamera();

	};
	
}
