#pragma once
#include <any>

namespace Polyboid
{

    class Imgui
    {

    public:
        static void Init(const std::any& window);
        static void Begin();
        static void End();
        static void ShutDown();
    
    };

}

