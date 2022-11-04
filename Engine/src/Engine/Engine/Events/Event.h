#pragma once
#include <functional>

namespace Polyboid
{

	enum class EventType
	{
		//Window Events
		NONE,
		KEY,
		MOUSE,
		WINDOW_CLOSE,
		WINDOW_RESIZE,
		FRAME_BUFFER_RESIZE,
		MOUSE_SCROLL,

		//Editor Events
		ON_EDITOR_PLAY_MODE_ENTER,
		ON_EDITOR_PLAY_MODE_EXIT
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


