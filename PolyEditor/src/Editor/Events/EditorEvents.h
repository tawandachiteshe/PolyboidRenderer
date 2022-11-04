#pragma once
#include "Engine/Engine/Events/Event.h"

namespace Polyboid
{
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
