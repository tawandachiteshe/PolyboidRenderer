#pragma once
#include <functional>
#include <spdlog/spdlog.h>

#include "Event.h"
#include "Engine/Engine/Base.h"

namespace Polyboid
{
    
  
    class EventDispatcher
    {
    private:
        EventType m_Type;
        Event m_Event;
        
    public:

        EventDispatcher(const Event& event);

        template<typename TEventType>
        void Bind(const std::function<void(const TEventType& event)>& fun);
        
    };

    template <typename TEventType>
    void EventDispatcher::Bind(const std::function<void(const TEventType& event)>& fun)
    {
        switch (m_Type)
        {
        case EventType::NONE: break;
        case EventType::KEY: break;
        case EventType::MOUSE: break;
        case EventType::WINDOW_CLOSE: fun(CastEventAs<TEventType>(m_Event)); break;
        case EventType::WINDOW_RESIZE: break;
        case EventType::FRAME_BUFFER_RESIZE: break;
        case EventType::MOUSE_SCROLL: break;
        default: ;
        }
        
    }
}
