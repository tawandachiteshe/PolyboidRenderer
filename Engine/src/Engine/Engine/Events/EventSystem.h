#pragma once
#include <functional>
#include <spdlog/spdlog.h>

#include "Event.h"
#include "Engine/Engine/Base.h"


namespace Polyboid
{

    using EventHandlerFn = void(const Event&);
    
    class EventDispatcher
    {
    public:

        void AddEventHandler(const EventType& type, const std::function<EventHandlerFn>& fun)
        {
            m_Handlers[type].emplace_back(fun);
        }

        void Dispatch(const Event& eventName)
        {
            auto type = eventName.GetType();
            if (m_Handlers.find(type) == m_Handlers.end())
                return;


            auto&& handlers = m_Handlers.at(type);


            for (auto&& handler : handlers)
            {
                handler(eventName);
            }

        }

    private:
       
        std::unordered_map<EventType, std::vector<std::function<EventHandlerFn>>> m_Handlers;
    };


    class EventSystem
    {
    
    public:
        static Unique<EventDispatcher> s_Dispatcher;
        static Unique<EventDispatcher>& GetDispatcher() { return s_Dispatcher; }

        static void Init();

        static void Bind(const EventType& type, const std::function<EventHandlerFn>& fun)
        {
        	s_Dispatcher->AddEventHandler(type, fun);
        }

        friend class Application;
    };

}
