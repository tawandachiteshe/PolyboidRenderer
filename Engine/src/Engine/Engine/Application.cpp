#include "boidpch.h"


#include "Application.h"
#include <spdlog/spdlog.h>

#include "PolyboidWindow.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/Swapchain.h"
#include <GLFW/glfw3.h>

#include "ImguiSetup.h"
#include "ECS/ECSManager.h"
#include "Engine/Renderer/Renderer2D.h"
#include "EntryPoint.h"
#include "Events/EventSystem.h"
#include "Events/WindowEvent.h"


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
        m_IsRunning = true;

        Imgui::Init();
        ECSManager::Init();

        //multiple overides maybe but is it efficieant and maintainable;;


        EventSystem::Bind(EventType::WINDOW_CLOSE, BIND_EVENT(OnWindowsCloseEvent));
        EventSystem::Bind(EventType::WINDOW_RESIZE, BIND_EVENT(OnWindowResizeEvent));
        

    }

    Application::~Application()
    {
        ShutDown();
    }


    void Application::OnWindowResizeEvent(const Event& event)
    {
        const auto _event = CastEventAs<WindowResizeEvent>(event);

        m_AppData.windowSpecs.Height = _event.GetHeight();
        m_AppData.windowSpecs.Width = _event.GetWidth();
    }

    void Application::OnWindowsCloseEvent(const Event& event)
    {
       
        m_IsRunning = false;
    }

    void Application::AddLayer(Layer* layer)
    {
        m_Layers.AddLayer(layer);
    }

    void Application::ShutDown()
    {
        Imgui::ShutDown();
        Renderer2D::Shutdown();
       
    }

    void Application::Run()
    {
        while (m_IsRunning)
        {

            m_Window->PollEvents();
           

            const double currentFrame = glfwGetTime();
            m_DeltaTime = currentFrame - m_LastFrame;

            // update here.....

            for (auto layer : m_Layers)
            {
                layer->OnUpdate(m_DeltaTime);
            }
            
            m_LastFrame = currentFrame;

            //rendering here////

            Imgui::Begin();

            //imgui here....
            for (auto layer : m_Layers)
            {
                layer->OnImguiRender();
            }

        	Imgui::End();


            m_Swapchain->SwapBuffers();
            
           
        }
    }
}
