#include "boidpch.h"


#include "Application.h"
#include <spdlog/spdlog.h>

#include "Engine.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/Swapchain.h"

#include "ImguiSetup.h"
#include "Engine/Renderer/Renderer2D.h"
#include "EntryPoint.h"
#include "imgui.h"
#include "Engine/Renderer/RenderPass.h"
#include "Engine/Renderer/CommandList/RenderCommand.h"
#include "Events/EventDispatcher.h"
#include "Events/WindowEvent.h"
#include "GLFW/glfw3.h"
#include "Registry/ShaderRegistry.h"


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
		mainWindowSettings.NoApi = true;

		m_MainWindow = Window::Create(mainWindowSettings);

	
		m_Running = true;

		//multiple overides maybe but is it efficieant and maintainable;;
		m_MainWindow->SetEventCallback(BIND_EVENT(Application::OnEvent));

		const auto nativeWindow = m_MainWindow->GetNativeWindow();
		m_RenderAPI = RenderAPI::Create(RenderAPIType::Vulkan, nativeWindow);
	}

	Application::~Application()
	{
		ShutDown();
		delete m_RenderAPI;
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
		m_Swapchain->Resize(event.GetWidth(), event.GetHeight());
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
		Imgui::ShutDown();
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

	
		Renderer::Init(m_RenderAPI);
		ShaderRegistry::Init(m_RenderAPI);

		

		SwapchainSettings settings{};
		settings.Width = m_Settings.WindowWidth;
		settings.Height = m_Settings.WindowHeight;
		settings.SwapchainFormat = EngineGraphicsFormats::BGRA8ISrgb;
		

		m_Swapchain = m_RenderAPI->CreateSwapChain(settings);
		m_RenderAPI->CreateTexture2D({ .sizedFormat = EngineGraphicsFormats::Depth24Stencil8, .Width = 1600, .Height = 900 });

		Imgui::Init(m_MainWindow->GetNativeWindow());

		while (m_Running)
		{
			OPTICK_FRAME("Render Frame")


			m_MainWindow->PollEvents();



			Renderer::BeginFrame();
			Renderer::BeginCommands();
			Renderer::BeginRenderPass(m_Swapchain->GetDefaultRenderPass());
			Renderer::ClearRenderPass(glm::vec4(1, 0, 0, 1));


			Imgui::Begin();
			ImGui::ShowDemoWindow();
			Imgui::End();
			/// <summary>
			///
			/// </summary>

			Renderer::EndRenderPass();
			Renderer::EndCommands();
			Renderer::WaitAndRender();
			m_Swapchain->SwapBuffers();
			Renderer::EndFrame();


	

		}

	}
}
