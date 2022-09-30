#include "Swapchain.h"

#include <GLFW/glfw3.h>
#include "Engine/Core/PolyboidWindow.h"


namespace Polyboid
{
    Swapchain::Swapchain(GLFWwindow* window): m_Window(window)
    {
        
    }

    void Swapchain::SwapBuffers()
    {
        glfwSwapBuffers(m_Window);
    }

    void Swapchain::SetVsync(bool vsync)
    {
        if (vsync)
        {
            glfwSwapInterval(0);
        }
        else
        {
            glfwSwapInterval(1);
        }
        
    }

    std::shared_ptr<Swapchain> Swapchain::MakeSwapChain(GLFWwindow* window)
    {
        return std::make_shared<Swapchain>(window);
    }
}

