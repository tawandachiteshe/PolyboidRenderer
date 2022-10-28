#pragma once
#include "Engine/Engine/PolyboidWindow.h"

namespace Polyboid
{
    class Swapchain
    {
    private:
        GLFWwindow* m_Window;
       
    public:
        Swapchain(GLFWwindow* window);
        void SwapBuffers();
        void SetVsync(bool vsync);
        static std::shared_ptr<Swapchain> MakeSwapChain(GLFWwindow* window);
    
    };

}
