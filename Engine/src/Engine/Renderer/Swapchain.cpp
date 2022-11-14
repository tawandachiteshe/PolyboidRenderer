#include "boidpch.h"

#include "Swapchain.h"

#include <GLFW/glfw3.h>
#include "Engine/Engine/PolyboidWindow.h"
#include "Engine/Engine/Debug/Profiler.h"


namespace Polyboid
{
    Swapchain::Swapchain(GLFWwindow* window): m_Window(window)
    {
        POLYBOID_PROFILE_FUNCTION();
    }

    void Swapchain::SwapBuffers()
    {
        POLYBOID_PROFILE_FUNCTION();

        glfwSwapBuffers(m_Window);
    }

    void Swapchain::SetVsync(bool vsync)
    {
        POLYBOID_PROFILE_FUNCTION();
 
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
        POLYBOID_PROFILE_FUNCTION();

        return std::make_shared<Swapchain>(window);
    }
}

