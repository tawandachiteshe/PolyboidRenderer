#pragma once

#include "PolyboidWindow.h"
#include "KeyCodes.h"
#include "Engine/Renderer/Swapchain.h"
#include <Engine/Engine/LayerContainer.h>


int main(int argc, char** argv);
namespace Polyboid
{
    struct ApplicationData
    {
        ApplicationData() = default;
        WindowSpecs windowSpecs = {};
        ApplicationData(WindowSpecs specs): windowSpecs(std::move(specs)){}
        
    };

    struct ApplicationStats
    {
        double m_GameTime = 0.0;
        double m_LastGameFrame = 0.0;
        double m_RenderTime = 0.0;
        double m_LastRenderFrame = 0.0;
    };
    
    class Application
    {
    public:
        Application();
        virtual ~Application();

		std::unique_ptr<PolyboidWindow>& GetWindow() { return m_Window; }
    	static Application* Get() { return s_Instance; }
        ApplicationData& GetAppData() { return  m_AppData; }
        WindowSpecs& GetWindowSpecs() { return m_AppData.windowSpecs; }
        ApplicationStats& GetStats() { return stats; }
        void SubmitToRenderThread(const std::function<void()>& function);
        void RenderThreadQueue();

        
    protected:
        
        ApplicationData m_AppData = WindowSpecs(1280, 800, "Polyboid");
    	void OnWindowResizeEvent(const Event& event);
    	void OnWindowsCloseEvent(const Event& event);
        void AddLayer(Layer* layer);


    private:
      
        LayerContainer m_Layers;
        std::unique_ptr<PolyboidWindow> m_Window;
        std::shared_ptr<Swapchain> m_Swapchain;
        void Run();
        void Render();
        bool m_IsRunning = false;
        static void ShutDown();
        ApplicationStats stats {};

        std::vector<std::function<void()>> m_RenderThreadQueue;
        std::mutex m_RenderMutex;

   

    private:
        static Application* s_Instance;
        friend int ::main(int argc, char** argv);
        
       
    
    };



    Application* CreateApplication();
}

