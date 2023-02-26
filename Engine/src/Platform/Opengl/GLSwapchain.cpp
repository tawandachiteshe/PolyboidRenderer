#include "boidpch.h"
#include "GLSwapchain.h"

#include "GLFW/glfw3.h"


namespace Polyboid
{
    GLSwapchain::GLSwapchain(const std::any& window): m_Window(window)
    {
        
    }

    void GLSwapchain::SwapBuffers()
    {
        glfwSwapBuffers(std::any_cast<GLFWwindow*>(m_Window));
    }

    void GLSwapchain::SetVsync(bool vsync)
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
}
