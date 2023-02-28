#include "boidpch.h"


#include "Application.h"
#include <spdlog/spdlog.h>

#include "Engine.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/Swapchain.h"

#include "ImguiSetup.h"
#include "Engine/Renderer/Renderer2D.h"
#include "EntryPoint.h"
#include "Engine/Renderer/CommandList/RenderCommand.h"
#include "Events/EventDispatcher.h"
#include "Events/WindowEvent.h"
#include "GLFW/glfw3.h"


namespace Polyboid
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSettings& settings) : m_Settings(settings)
	{
		OPTICK_EVENT("Polyboid App init");
		spdlog::info("App init");

		Init(settings);

		s_Instance = this;
	}

	Application::Application()
	{
		OPTICK_EVENT("Polyboid App init");
		spdlog::info("App init");

		Init(m_Settings);

		s_Instance = this;
	}


	void Application::Init(const ApplicationSettings& settings)
	{
		//Init Windows
		WindowSettings mainWindowSettings(true,
		                                  settings.WindowWidth,
		                                  settings.WindowHeight,
		                                  settings.ApplicationName);

		m_MainWindow = Window::Create(mainWindowSettings);

	
		m_Running = true;

		//multiple overides maybe but is it efficieant and maintainable;;
		m_MainWindow->SetEventCallback(BIND_EVENT(Application::OnEvent));
	}

	Application::~Application()
	{
		ShutDown();
	}


	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		
		dispatcher.Bind<WindowCloseEvent>(BIND_EVENT(Application::OnWindowsCloseEvent));
		dispatcher.Bind<WindowResizeEvent>(BIND_EVENT(Application::OnWindowResizeEvent));

		for (const auto& layer : m_Layers)
		{
			layer->OnEvent(event);
		}

	}

	void Application::OnWindowResizeEvent(WindowResizeEvent& event)
	{
		m_Settings.WindowHeight = event.GetHeight();
		m_Settings.WindowWidth = event.GetWidth();
	}

	void Application::OnWindowsCloseEvent(WindowCloseEvent& event)
	{
		m_Running = false;
		spdlog::info("Window close event: {}", (uint32_t)event.GetType());
	}

	void Application::AddLayer(Layer* layer)
	{
		m_Layers.AddLayer(layer);
	}

	void Application::ShutDown()
	{
	}

	void Application::Run()
	{
		OPTICK_THREAD("Main Thread")
		m_RenderThread = std::thread([&]()
		{
			Render();
		});

		Engine::Init();


		while (m_Running)
		{
			OPTICK_FRAME("Main Frame")

			const double currentFrame = glfwGetTime();
			double m_GameTime = currentFrame - m_LastFrameTime;
			m_LastFrameTime = currentFrame;


			// update here.....
			for (auto layer : m_Layers)
			{
				layer->OnUpdate(static_cast<float>(m_GameTime));
			}

			m_MainWindow->PollEvents();

			m_ShouldRender = true;
			
		}

		m_RenderThread.join();
	}

	void Application::Render()
	{
		OPTICK_THREAD("Render Thread")

		const auto nativeWindow = m_MainWindow->GetNativeWindow();
		m_RenderAPI = RenderAPI::Create(RenderAPIType::Opengl, nativeWindow);
		const auto swapChain = m_RenderAPI->CreateSwapChain(nativeWindow);
		swapChain->SetVsync(true);
		RenderCommand::Init(m_RenderAPI);

		Imgui::Init(nativeWindow);
		Engine::InitRenderer(m_RenderAPI);


		while (m_Running)
		{
			OPTICK_FRAME("Render Frame")

			for (const auto layer : m_Layers)
			{
				layer->OnRender();
			}

			
			if (m_ShouldRender)
			{
				Renderer::WaitAndRender();
			}


			Imgui::Begin();

			//imgui here....
			for (const auto layer : m_Layers)
			{
				layer->OnImguiRender();
			}

			Imgui::End();


			swapChain->SwapBuffers();

			m_ShouldRender = false;
		}

		Imgui::ShutDown();
	}
}
