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
        ~GLSwapchain() override;
        void Resize(uint32_t width, uint32_t height) override;
        Ref<RenderPass> GetDefaultRenderPass() override;
    };

}

