#include "boidpch.h"
#include "GameStatics.h"

#include "Engine/Engine/Events/EventSystem.h"

namespace Polyboid
{

	Ref<GameInstanceData> GameStatics::m_sData = std::make_shared<GameInstanceData>();

	void GameStatics::SetCurrentCamera(const Ref<Camera>& camera)
	{
		
		m_sData->m_CurrentCamera = camera;
	}

	void GameStatics::SetCurrentWorld(const Ref<World>& world)
	{
		
		m_sData->m_CurrentWorld = world;
	}

	Ref<World>& GameStatics::GetCurrentWorld()
	{
		
		return  m_sData->m_CurrentWorld;
	}


	Ref<Camera>& GameStatics::GetCurrentCamera()
	{
		
		return m_sData->m_CurrentCamera;
	}
}

