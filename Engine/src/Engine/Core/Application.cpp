#include "Application.h"
#include <spdlog/spdlog.h>

#include "PolyboidWindow.h"
#include "Engine/Renderer/Context.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/Swapchain.h"
#include <GLFW/glfw3.h>


namespace Polyboid
{

    Application::Application()
    {
        spdlog::info("App init");
      
        m_Window = PolyboidWindow::MakeWindow(m_AppData.windowSpecs);
        const auto nativeWindow = m_Window->GetNativeWindow();
        
        m_Swapchain = Swapchain::MakeSwapChain(nativeWindow);
        m_Swapchain->SetVsync(true);

        Renderer::Init();
    }

    Application::~Application()
    {
        
    }

    

    void Application::Run()
    {
        
        while (m_Window->Run())
        {

            double currentFrame = glfwGetTime();
            m_DeltaTime = currentFrame - m_LastFrame;

            Update(m_DeltaTime);
            
            m_LastFrame = currentFrame;

            //rendering here////

            m_Swapchain->SwapBuffers();
            
            m_Window->PollEvents();
        }
    }
}
