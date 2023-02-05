#include "boidpch.h"


#include "Application.h"
#include <spdlog/spdlog.h>

#include "PolyboidWindow.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/Swapchain.h"
#include <GLFW/glfw3.h>

#include "AssetManager.h"
#include "ImguiSetup.h"
#include "ECS/ECSManager.h"
#include "Engine/Renderer/Renderer2D.h"
#include "EntryPoint.h"
#include "Debug/Timer.h"
#include "Events/EventSystem.h"
#include "Events/WindowEvent.h"
#include "Scripting/ScriptingEngine.h"
#include "Engine/Renderer/Context.h"


namespace Polyboid
{

    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        POLYBOID_PROFILE_FUNCTION();
        
        spdlog::info("App init");

        s_Instance = this;
        
        m_Window = PolyboidWindow::MakeWindow(m_AppData.windowSpecs);
      
        m_IsRunning = true;

       
        ECSManager::Init();
        ScriptingEngine::Init();
        AssetManager::Init();

        //multiple overides maybe but is it efficieant and maintainable;;


        EventSystem::Bind(EventType::WINDOW_CLOSE, BIND_EVENT(OnWindowsCloseEvent));
        EventSystem::Bind(EventType::WINDOW_RESIZE, BIND_EVENT(OnWindowResizeEvent));
        

    }

    Application::~Application()
    {
        POLYBOID_PROFILE_FUNCTION();
        ShutDown();
    }


    void Application::SubmitToRenderThread(const std::function<void()>& function)
    {
        m_RenderThreadQueue.push_back(function);
    }

    void Application::RenderThreadQueue()
    {
        std::scoped_lock lock(m_RenderMutex);

	    for (auto& func : m_RenderThreadQueue)
	    {
            func();
	    }

        m_RenderThreadQueue.clear();
    }

    void Application::OnWindowResizeEvent(const Event& event)
    {
        POLYBOID_PROFILE_FUNCTION();
        const auto _event = CastEventAs<WindowResizeEvent>(event);

        m_AppData.windowSpecs.Height = _event.GetHeight();
        m_AppData.windowSpecs.Width = _event.GetWidth();
    }

    void Application::OnWindowsCloseEvent(const Event& event)
    {
        POLYBOID_PROFILE_FUNCTION();
        m_IsRunning = false;
    }

    void Application::AddLayer(Layer* layer)
    {
        POLYBOID_PROFILE_FUNCTION();
        m_Layers.AddLayer(layer);
    }

    void Application::ShutDown()
    {
        POLYBOID_PROFILE_FUNCTION();

        ScriptingEngine::ShutDown();
       
    }

    void Application::Run()
    {
        POLYBOID_PROFILE_FUNCTION();

        std::thread renderThread([&]()
            {
                Render();
            });


        double lastTime = glfwGetTime();
        int nbFrames = 0;

        while (m_IsRunning)
        {

            double currentTime = glfwGetTime();
            nbFrames++;
            if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
                // printf and reset timer
                stats.GameTimeMs = 1000.0 / double(nbFrames);
                nbFrames = 0;
                lastTime += 1.0;
            }


            const double currentFrame = glfwGetTime();
            stats.m_GameTime = currentFrame - stats.m_LastGameFrame;
            stats.m_LastGameFrame = currentFrame;

            // update here.....

            {
                POLYBOID_PROFILE_SCOPE("Update");

                for (auto layer : m_Layers)
                {
                    layer->OnUpdate(stats.m_GameTime);
                }
            }

            m_Window->PollEvents();


        }

        renderThread.detach();
    }

    void Application::Render()
    {


        const auto context = Context::MakeContext();
        const auto nativeWindow = m_Window->GetNativeWindow();

        context->MakeCurrent(nativeWindow);

        m_Swapchain = Swapchain::MakeSwapChain(nativeWindow);
        m_Swapchain->SetVsync(true);

      
        Imgui::Init();
    	Renderer::Init();

        double lastTime = glfwGetTime();
        int nbFrames = 0;

        while (m_IsRunning)
        {

            const double currentFrame = glfwGetTime();
            stats.m_RenderTime = currentFrame - stats.m_LastRenderFrame;
            stats.m_LastRenderFrame = currentFrame;

            double currentTime = glfwGetTime();
            nbFrames++;
            if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
                // printf and reset timer
                stats.RenderTimeMs = 1000.0 / double(nbFrames);
                nbFrames = 0;
                lastTime += 1.0;
            }

            RenderThreadQueue();

            for (auto layer : m_Layers)
            {
                layer->OnRender(stats.m_RenderTime);
            }

        	Imgui::Begin();
		    
		    //imgui here....
		    for (auto layer : m_Layers)
		    {
		        layer->OnImguiRender();
		    }
		    
		    Imgui::End();

            m_Swapchain->SwapBuffers();

            
        }

        Imgui::ShutDown();
		Renderer2D::Shutdown();
        
    }
}
