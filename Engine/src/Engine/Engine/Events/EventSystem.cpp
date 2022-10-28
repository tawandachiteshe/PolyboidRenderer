#include "EventSystem.h"


namespace Polyboid
{
    Unique<EventDispatcher> EventSystem::s_Dispatcher = std::make_unique<EventDispatcher>();

    void EventSystem::Init()
    {

    }
}
