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
    
    class Application
    {
    public:
        Application();
        virtual ~Application();

		std::unique_ptr<PolyboidWindow>& GetWindow() { return m_Window; }
    	static Application* Get() { return s_Instance; }
        
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
        bool m_IsRunning = false;
        static void ShutDown();

        double m_DeltaTime = 0.0;
        double m_LastFrame = 0.0;

        
    private:
        static Application* s_Instance;
        friend int ::main(int argc, char** argv);
        
       
    
    };



    Application* CreateApplication();
}

