#pragma once
#include <any>
#include <functional>
#include <spdlog/spdlog.h>

#include "Event.h"
#include "MouseEvents.h"
#include "Engine/Engine/Base.h"

namespace Polyboid
{
    
  
    class EventDispatcher
    {
    private:
        EventType m_Type;
        Event& m_Event;

        std::unordered_map<EventType, std::any> m_EventCallBacks;

    public:

        EventDispatcher(Event& event);

        template<typename T, typename F>
        void Bind(const F& fun);
        
    };

    template<typename T, typename F>
    void EventDispatcher::Bind(const F& fun)
    {
        m_EventCallBacks[m_Type] = fun;

        if (m_Type == T::GetStaticType())
        {
            fun(static_cast<T&>(m_Event));
        }
       
        
    }
}
