#pragma once

#include "PolyboidWindow.h"
#include "KeyCodes.h"
#include "Engine/Renderer/Swapchain.h"


int main(int argc, char** argv);
namespace Polyboid
{
    struct ApplicationData
    {
        ApplicationData() = default;
        WindowSpecs windowSpecs = {};
        ApplicationData(const WindowSpecs& specs): windowSpecs(specs){}
        
    };
    
    class Application
    {
    public:
        Application();
        virtual ~Application();

       std::unique_ptr<PolyboidWindow>& GetWindow() { return m_Window; }
        static Application* Get() { return s_Instance; }
        
    protected:
        
        ApplicationData m_AppData = WindowSpecs(1280, 800, "Polyboid");
        virtual  void OnKeyEvent(KeyCodes codes, KeyAction action);
        virtual  void OnMouseEvent(MouseCodes codes, KeyAction action);
        virtual void OnWindowResizeEvent(uint32_t width, uint32_t height);
        virtual void OnFrameBufferResizeEvent(uint32_t width, uint32_t height);
        virtual void OnWindowsCloseEvent();
        virtual void OnMouseScrollEvent(double offset);
        
        
        
    private:
      
        std::unique_ptr<PolyboidWindow> m_Window;
        std::shared_ptr<Swapchain> m_Swapchain;
        void Run();
        virtual void Update(float deltaTime) = 0;
        bool m_IsRunning = false;
        void ShutDown();

        double m_DeltaTime = 0.0;
        double m_LastFrame = 0.0;

        
    private:
        static Application* s_Instance;
        friend int ::main(int argc, char** argv);
        
       
    
    };



    inline void Application::OnMouseEvent(MouseCodes codes, KeyAction action)
    {
    }

    Application* CreateApplication();
}

