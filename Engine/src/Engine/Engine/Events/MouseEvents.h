#pragma once
#include "Event.h"


namespace Polyboid
{
	class MouseScrollEvent: public Event
	{
	private:
		float m_Yoffset;
	public:
		MouseScrollEvent(const float yOffset): m_Yoffset(yOffset)
		{
			m_Type = EventType::MOUSE_SCROLL;
		}

		float GetYoffset() const
		{
			return m_Yoffset;
		}

		ADD_STATIC_TYPE(MOUSE_SCROLL)
	};

}
