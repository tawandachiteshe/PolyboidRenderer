#pragma once
#include "Event.h"


namespace Polyboid
{
	class MouseScrollEvent: public Event
	{
	private:
		float m_Yoffset;
	public:
		MouseScrollEvent(float yOffset): m_Yoffset(yOffset)
		{
			m_Type = EventType::MOUSE_SCROLL;
		}

		float GetYoffset() { return m_Yoffset; }
	};

}
