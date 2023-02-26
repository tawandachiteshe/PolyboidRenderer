#include "boidpch.h"
#include "WindowsWindow.h"

#include "Engine/Engine/Events/WindowEvent.h"


namespace Polyboid
{
    static uint32_t s_WindowCount = 0;

    WindowData& WindowsWindow::GetWindowData(GLFWwindow* window)
    {
        return *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
    }

    WindowsWindow::WindowsWindow(const WindowSettings& settings): m_Settings(settings)
    {
        if (settings.IsMainWindow)
        {
            if (!glfwInit())
            {
                //TODO: Wrap this in some kind of a macro: I mean copy cherno's code
                __debugbreak();
            }
        }
        
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


        if (settings.IsVisible)
        {
            //Redudant i know bro;
            glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
        }
        else
        {
            glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        }
        
        if (settings.WindowShareHandle.has_value())
        {
            m_Handle = glfwCreateWindow(static_cast<int32_t>(settings.Width),
                                        static_cast<int32_t>(settings.Height),
                                        settings.WindowTitle.c_str(),
                                        nullptr,
                                        std::any_cast<GLFWwindow*>(settings.WindowShareHandle));
        }
        else
        {
            m_Handle = glfwCreateWindow(static_cast<int32_t>(settings.Width),
                                        static_cast<int32_t>(settings.Height),
                                        settings.WindowTitle.c_str(),
                                        nullptr,
                                        nullptr);
        }

        if (m_Handle == nullptr)
        {
            __debugbreak();
        }

        glfwSetWindowUserPointer(m_Handle, &m_Data);

        //Bind Window Events here
        glfwSetWindowCloseCallback(m_Handle, [](GLFWwindow* window)
        {
            auto& data = GetWindowData(window);
            const WindowCloseEvent windowClose;
            data.EventCallback(windowClose);
        });
        
    }

    uint32_t WindowsWindow::GetWidth()
    {
        return m_Settings.Width;
    }

    uint32_t WindowsWindow::GetHeight()
    {
        return m_Settings.Height;
    }

    void WindowsWindow::PollEvents()
    {
        glfwPollEvents();
    }

    void WindowsWindow::SetEventCallback(const EventFn& fn)
    {
        m_Data.EventCallback = fn;
    }

    //Explicit nullptr handling
    std::any WindowsWindow::GetNativeWindow()
    {
        
        return m_Handle;
    }

    WindowsWindow::~WindowsWindow()
    {
        glfwDestroyWindow(m_Handle);
        if (m_Settings.IsMainWindow)
        {
            glfwTerminate();
        }
    }
}
