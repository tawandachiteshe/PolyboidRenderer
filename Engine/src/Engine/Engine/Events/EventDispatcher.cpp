#include "boidpch.h"

#include "EventDispatcher.h"


namespace Polyboid
{
    EventDispatcher::EventDispatcher(const Event& event): m_Event(event)
    {
        m_Type = event.GetType();
    }

   
}
