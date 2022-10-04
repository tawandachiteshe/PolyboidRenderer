#pragma once
#include "Engine/Core/Application.h"

namespace Polyboid
{
    class EventSystem
    {
    
    public:
        static void Init();
    
        friend Polyboid::Application;
    };

}
