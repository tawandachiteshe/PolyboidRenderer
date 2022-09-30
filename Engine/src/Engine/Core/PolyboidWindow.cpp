#include "PolyboidWindow.h"

#include <spdlog/spdlog.h>

#include "Engine/Renderer/Context.h"
#include "GLFW/glfw3.h"


namespace Polyboid
{
    static void error_callback(int error, const char* description)
    {
        spdlog::error("error code: {0} \n error description {1}", error, description);
    }
    
    PolyboidWindow::PolyboidWindow(const WindowSpecs& specs)
    {
        if (!glfwInit())
        {
            __debugbreak();
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwSetErrorCallback(error_callback);

        
        auto context = Context::MakeContext();
        
        spdlog::info("Creating windows...");

        m_Window = glfwCreateWindow(specs.Width, specs.Height, specs.Title.c_str(), NULL, NULL);

        if (!m_Window)
        {
            glfwTerminate();
            __debugbreak();
        }

        context->MakeCurrent(m_Window);

       
        
    }

    std::unique_ptr<PolyboidWindow> PolyboidWindow::MakeWindow(const WindowSpecs& specs)
    {
        return std::make_unique<PolyboidWindow>(specs);
    }

    bool PolyboidWindow::Run()
    {
        m_IsRunning = !glfwWindowShouldClose(m_Window);
        return m_IsRunning;
        
    }

    void PolyboidWindow::PollEvents()
    {
        glfwPollEvents();
        
    }

    PolyboidWindow::~PolyboidWindow()
    {
        glfwTerminate();
    }
}

