#pragma once
#include <functional>

namespace Polyboid
{

	enum class EventType
	{
		//Window Events
		NONE = 0,
		KEY,
		MOUSE,
		WINDOW_CLOSE,
		WINDOW_RESIZE,
		FRAME_BUFFER_RESIZE,
		MOUSE_SCROLL,
	};

	class Event
	{
	protected:
		EventType m_Type = EventType::NONE;

	public:
		Event() = default;
		EventType GetType() const { return m_Type; }
		virtual ~Event() = default;
	};

	using EventFn = std::function<void(const Event& event)>;

}


