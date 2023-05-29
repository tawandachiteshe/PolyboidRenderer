#include "boidpch.h"
#include "GameStatics.h"

#include "Engine/Engine/Events/EventDispatcher.h"

namespace Polyboid
{

	Ref<GameInstanceData> GameStatics::m_sData = CreateRef<GameInstanceData>();

	void GameStatics::SetCurrentCamera(const Ref<Camera>& camera)
	{
		
		m_sData->m_CurrentCamera = camera;
	}

	void GameStatics::SetCurrentWorld(const Ref<World>& world)
	{
		
		m_sData->m_CurrentWorld = world;
	}

	void GameStatics::SetWindowOffset(const glm::vec2& offset)
	{
		m_sData->windowOffset = offset;
	}

	glm::vec2 GameStatics::GetWindowOffset()
	{
		return  m_sData->windowOffset;
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

