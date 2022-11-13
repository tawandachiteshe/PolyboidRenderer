#pragma once
#include "Engine/Engine/Events/Event.h"

namespace Polyboid
{
	class GameObject;


	class GameObjectOutlineClick : public Event
	{

	private:
		GameObject* gameObjectId;

	public:
		GameObjectOutlineClick(GameObject* gameObject) : gameObjectId(gameObject)
		{
			m_Type = EventType::ON_GAME_OBJECT_SELECTED;
		}

		GameObject* GetGameObject() { return gameObjectId; }

	};

	class GameObjectOutlineDeleted : public Event
	{
	public:
		GameObjectOutlineDeleted()
		{
			m_Type = EventType::ON_GAME_OBJECT_DELETED;
		}


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
