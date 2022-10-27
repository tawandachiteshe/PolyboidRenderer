﻿#include "PolyboidWindow.h"

#include <spdlog/spdlog.h>

#include "KeyCodes.h"
#include "Engine/Renderer/Context.h"
#include "Events/EventSystem.h"
#include "Events/WindowEvent.h"
#include "GLFW/glfw3.h"


namespace Polyboid
{
    static void error_callback(int error, const char* description)
    {
        spdlog::error("error code: {0} \n error description {1}", error, description);
    }

    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
    }

    static void OnFrameBufferResize(GLFWwindow* window, int width, int height)
    {

    }

    static void OnWindowResize(GLFWwindow* window, int width, int height)
    {
    	const WindowResizeEvent windowResize(width, height);
    	EventSystem::GetDispatcher()->Dispatch(windowResize);
    }


    static void OnWindowShouldClose(GLFWwindow* window)
    {
    	const WindowCloseEvent windowCloseEvent;
    	EventSystem::GetDispatcher()->Dispatch(windowCloseEvent);
    }

    static void OnMouseCursorCallback(GLFWwindow* window, double xpos, double ypos)
    {
    }

    static void OnWindowFocusCallback(GLFWwindow* window, int focused)
    {
    }


    void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
    
    }

    void MouseScrollCallBack(GLFWwindow* window, double xoffset, double yoffset)
    {
      
    }

    std::unique_ptr<WindowData> PolyboidWindow::s_WindowData = std::make_unique<WindowData>();

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


        const auto context = Context::MakeContext();

        spdlog::info("Creating windows...");

        m_Window = glfwCreateWindow(specs.Width, specs.Height, specs.Title.c_str(), NULL, NULL);

        glfwSetKeyCallback(m_Window, KeyCallback);
        glfwSetFramebufferSizeCallback(m_Window, OnFrameBufferResize);
        glfwSetWindowCloseCallback(m_Window, OnWindowShouldClose);
        glfwSetWindowFocusCallback(m_Window, OnWindowFocusCallback);
        glfwSetWindowSizeCallback(m_Window, OnWindowResize);
        glfwSetCursorPosCallback(m_Window, OnMouseCursorCallback);
        glfwSetMouseButtonCallback(m_Window, MouseButtonCallback);
        glfwSetScrollCallback(m_Window, MouseScrollCallBack);
        
        
        glfwSetWindowUserPointer(m_Window, s_WindowData.get());

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
    

    void PolyboidWindow::PollEvents()
    {
        glfwPollEvents();
    }

    PolyboidWindow::~PolyboidWindow()
    {
        glfwTerminate();
    }
}
