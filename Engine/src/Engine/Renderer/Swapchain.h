#pragma once
#include <any>

#include "Engine/Engine/Base.h"


namespace Polyboid
{
    class Swapchain
    {
    public:
        virtual void SwapBuffers() = 0;
        virtual void SetVsync(bool vsync)= 0;
        virtual ~Swapchain() = default;

        //TODO: Make it robust
        static Ref<Swapchain> Create(const std::any& window);
    };

}
