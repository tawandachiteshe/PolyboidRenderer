#pragma once
#include "Event.h"

namespace Polyboid
{
	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent()
		{
			m_Type = EventType::WINDOW_CLOSE;
		}

		ADD_STATIC_TYPE(WINDOW_CLOSE)
	};


		class WindowResizeEvent : public Event
		{
		private:
			uint32_t width;
			uint32_t height;

		public:

			uint32_t GetWidth() const { return width; }
			uint32_t GetHeight() const { return height; }

			WindowResizeEvent(uint32_t width, uint32_t height) : width(width), height(height) { m_Type = EventType::WINDOW_RESIZE; }

			ADD_STATIC_TYPE(WINDOW_RESIZE)
		};


}
