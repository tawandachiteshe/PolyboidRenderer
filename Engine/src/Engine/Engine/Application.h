#pragma once


#include <Engine/Engine/LayerContainer.h>

#include "Window.h"
#include "Events/WindowEvent.h"
#include <thread>
#include <string>


#define CREATE_APPLICATION(Klass, variableName, ...)  static auto* variableName = new Klass(##__VA_ARGS__);					
    

int main(int argc, char** argv);

namespace Polyboid
{
	class RenderPass;
	class Swapchain;
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

		Unique<Window>& GetWindow() { return m_MainWindow; }

		static Application& Get() { return *s_Instance; }
		ApplicationSettings& GetAppSettings() { return m_Settings; }
		Ref<Swapchain> GetSwapchain() const { return m_Swapchain; }
		RenderAPI* GetRenderAPI() const { return  m_RenderAPI; }


	protected:
		Unique<Window> m_MainWindow = nullptr;
		RenderAPI* m_RenderAPI = nullptr;

		ApplicationSettings m_Settings;
		void OnEvent(Event& event);
		void OnWindowResizeEvent(WindowResizeEvent& event);
		void OnWindowsCloseEvent(WindowCloseEvent& event);
		void AddLayer(Layer* layer);


	private:
		std::thread m_RenderThread;
		LayerContainer m_Layers;
		Ref<Swapchain> m_Swapchain;
		

		void Run();
		void Render();
		static void ShutDown();
		std::atomic_bool m_Running = false;
		std::atomic_bool m_ShouldRender = true;

		double m_LastFrameTime = 0.0;

	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};


	Application* CreateApplication();

}
