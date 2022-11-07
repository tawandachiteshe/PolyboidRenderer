#include "boidpch.h"
#include "GameInstance.h"

namespace Polyboid
{

	Ref<GameInstanceData> GameInstance::m_sData = std::make_shared<GameInstanceData>();

	void GameInstance::SetCurrentCamera(const Ref<Camera>& camera)
	{
		m_sData->m_CurrentCamera = camera;
	}

	void GameInstance::SetCurrentWorld(World* world)
	{
		m_sData->m_CurrentWorld = world;
	}

	World* GameInstance::GetCurrentWorld()
	{
		return  m_sData->m_CurrentWorld;
	}


	Ref<Camera>& GameInstance::GetCurrentCamera()
	{
		return m_sData->m_CurrentCamera;
	}
}

