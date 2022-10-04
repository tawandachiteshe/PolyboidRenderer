#include "Application.h"
#include <spdlog/spdlog.h>

#include "PolyboidWindow.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/Swapchain.h"
#include <GLFW/glfw3.h>

#include "ImguiSetup.h"
#include "ECS/ECSManager.h"


namespace Polyboid
{

    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        spdlog::info("App init");

        s_Instance = this;
        
        m_Window = PolyboidWindow::MakeWindow(m_AppData.windowSpecs);
        const auto nativeWindow = m_Window->GetNativeWindow();

        Renderer::Init();
        m_Swapchain = Swapchain::MakeSwapChain(nativeWindow);
        m_Swapchain->SetVsync(true);
        
        PolyboidWindow::GetWindowData()->OnKeyEvent = [&] (KeyCodes key_codes, KeyAction action)
        {
            OnKeyEvent(key_codes, action);
        };

        PolyboidWindow::GetWindowData()->OnMouseEvent = [&](MouseCodes codes, KeyAction action)
        {
            OnMouseEvent(codes, action);
        };

        PolyboidWindow::GetWindowData()->OnWindowCloseEvent = [&]()
        {
            OnWindowsCloseEvent();
        };

        PolyboidWindow::GetWindowData()->OnFrameBufferResizeEvent = [&](uint32_t width, uint32_t height)
        {
            OnFrameBufferResizeEvent(width, height);
        };

        PolyboidWindow::GetWindowData()->OnWindowResizeEvent = [&](uint32_t width, uint32_t height)
        {
            OnWindowResizeEvent(width, height);
        };

        PolyboidWindow::GetWindowData()->OnMouseScrollEvent = [&](double offset)
        {
            OnMouseScrollEvent(offset);
        };

        m_IsRunning = true;

        Imgui::Init();
        ECSManager::Init();
        
    }

    Application::~Application()
    {
        ShutDown();
    }

    void Application::OnKeyEvent(KeyCodes codes, KeyAction action)
    {
        
    }

    void Application::OnWindowResizeEvent(uint32_t width, uint32_t height)
    {
        m_AppData.windowSpecs.Height = height;
        m_AppData.windowSpecs.Width = width;
    }

    void Application::OnFrameBufferResizeEvent(uint32_t width, uint32_t height)
    {
    }

    void Application::OnWindowsCloseEvent()
    {
       
        m_IsRunning = false;
    }

    void Application::OnMouseScrollEvent(double offset)
    {
    }

    void Application::ShutDown()
    {
        Imgui::ShutDown();
    }

    void Application::Run()
    {
        
        while (m_IsRunning)
        {

            m_Window->PollEvents();
            Imgui::Begin();

            const double currentFrame = glfwGetTime();
            m_DeltaTime = currentFrame - m_LastFrame;

            Update(static_cast<float>(m_DeltaTime));
            
            m_LastFrame = currentFrame;

            //rendering here////

            Imgui::End();
            m_Swapchain->SwapBuffers();
            
           
        }
    }
}
