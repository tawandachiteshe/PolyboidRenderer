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
	template <typename T>
	class MyAllocator {
	public:
		// Allocate memory for an object of type T
		T* allocate(size_t n) {
			return static_cast<T*>(std::malloc(n * sizeof(T)));
		}

		// Deallocate memory
		void deallocate(T* ptr, size_t n) {
			for (size_t i = 0; i < n; ++i) {
				ptr[i].~T();  // Call the destructor explicitly
			}
			std::free(ptr);
		}
	};


	class CommandBufferSet;
	class PipelineState;
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
		RenderAPI* GetRenderAPI() const { return  m_RenderAPI; }

	protected:
		Unique<Window> m_MainWindow = nullptr;
		RenderAPI* m_RenderAPI = nullptr;
		bool m_CanStartRendering = false;

		ApplicationSettings m_Settings;
		void OnEvent(Event& event);
		void OnWindowResizeEvent(WindowResizeEvent& event);
		void OnWindowsCloseEvent(WindowCloseEvent& event);
		void AddLayer(Layer* layer);


	private:
		LayerContainer m_Layers;

		void Run();
		void ShutDown();
		std::atomic_bool m_Running = false;
		std::atomic_bool m_ShouldRender = true;

		double m_LastFrameTime = 0.0;
		Ref<CommandBufferSet> m_CommandList = nullptr;

	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};


	Application* CreateApplication();

}
