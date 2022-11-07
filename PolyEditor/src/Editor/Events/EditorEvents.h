#pragma once
#include "Engine/Engine/Events/Event.h"

namespace Polyboid
{
	class GameObjectOutlineClick : public Event
	{

	private:
		uint32_t gameObjectId;

	public:
		GameObjectOutlineClick(uint32_t gameObject) : gameObjectId(gameObject)
		{
			m_Type = EventType::ON_GAME_OBJECT_SELECTED;
		}

		uint32_t GetGameObjectID() { return gameObjectId; }


		
	};

	class EditorPlayModeEnter : public Event
	{
	public:
		EditorPlayModeEnter()
		{
			m_Type = EventType::ON_EDITOR_PLAY_MODE_ENTER;
		}

	};

	class EditorPlayModeExit : public Event
	{
	public:
		EditorPlayModeExit()
		{
			m_Type = EventType::ON_EDITOR_PLAY_MODE_EXIT;
		}

	};

	
}
