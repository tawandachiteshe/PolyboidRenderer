#pragma once

#include "Engine/Renderer/Swapchain.h"
#include <Engine/Engine/LayerContainer.h>

#include "Window.h"
#include "Events/WindowEvent.h"
#include <thread>
#include <mutex>

#define ALLOC_APP(Klass, appName, ...) static auto* appName = new Klass(##__VA_ARGS__);


int main(int argc, char** argv);
namespace Polyboid
{
	class RenderAPI;

	struct ApplicationSettings
    {
        uint32_t WindowWidth = 1600;
        uint32_t WindowHeight = 900;
        std::string ApplicationName = "Polyboid";
    };

    
    class Application
    {
    private:
        Application(const ApplicationSettings& settings);
    public:
        Application();
        void Init(const ApplicationSettings& settings);
        virtual ~Application();

		Unique<Window>& GetWindow() { return m_GameWindow; }
        Ref<RenderAPI>& GetRenderAPI() { return m_RenderAPI; }

    	static Application& Get() { return *s_Instance; }
        ApplicationSettings& GetAppSettings() { return  m_Settings; }

        
    protected:

        Unique<Window> m_MainWindow = nullptr;
        Unique<Window> m_GameWindow = nullptr;
        Ref<RenderAPI> m_RenderAPI = nullptr;

        ApplicationSettings m_Settings;
        void OnEvent(const Event& event);
    	void OnWindowResizeEvent(const WindowResizeEvent& event);
    	void OnWindowsCloseEvent(const WindowCloseEvent& event);
        void AddLayer(Layer* layer);


    private:

        std::thread m_RenderThread;
        LayerContainer m_Layers;

        void Run();
        void Render();
        static void ShutDown();
        std::atomic_bool m_Running = false;

        double m_LastFrameTime = 0.0;

        std::mutex m_RenderMutex;
        std::mutex m_MainMutex;

    private:
        static Application* s_Instance;
        friend int ::main(int argc, char** argv);
        
       
    
    };

    Application* CreateApplication();
}

