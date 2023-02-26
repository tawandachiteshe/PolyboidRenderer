#pragma once
#include "Engine/Renderer/Swapchain.h"

namespace Polyboid
{
    class GLSwapchain : public Swapchain
    {
    private:
        std::any m_Window;
    public:
        GLSwapchain(const std::any& window);
        void SwapBuffers() override;
        void SetVsync(bool vsync) override;
    };

}

