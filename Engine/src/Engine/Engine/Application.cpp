#include "boidpch.h"


//#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "Application.h"
#include <spdlog/spdlog.h>

#include "Engine/Renderer/RenderCommand.h"

#include "ImguiSetup.h"
#include "Engine/Renderer/Renderer2D.h"
#include "EntryPoint.h"
#include "imgui.h"
#include "imgui_impl_vulkan.h"
#include "Engine/Renderer/CommandBufferSet.h"
#include "Engine/Renderer/KomputeCommand.h"
#include "Engine/Renderer/Renderer3D.h"
#include "Engine/Renderer/RenderPass.h"
#include "Engine/Renderer/VertexBufferArray.h"
#include "Events/EventDispatcher.h"
#include "Events/WindowEvent.h"
#include "GLFW/glfw3.h"
#include "Utils/SmartPtr.h"


namespace Polyboid
{
	class VkRenderAPI;
	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSettings& settings) : m_Settings(settings)
	{
		OPTICK_EVENT("Polyboid App init")
		spdlog::info("App init");

		Init(settings);

		s_Instance = this;
	}


	Application::Application()
	{
		OPTICK_EVENT("Polyboid App init")
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

		RenderCommand::Init(m_RenderAPI, m_Settings);
		KomputeCommand::Init();
		ShaderRegistry::Init(m_RenderAPI);
		Imgui::Init(m_MainWindow->GetNativeWindow());


		m_MainSwapChainCommandBuffer = CommandBufferSet::Create({ 3, CommandType::ManyTime });
		RenderCommand::PushCommandBufferSets({ m_MainSwapChainCommandBuffer });

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
		RenderCommand::Resize(m_Settings.WindowWidth, m_Settings.WindowHeight);
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
		//m_RenderAPI->Destroy();

	}

	void Application::Run()
	{

		

		while (m_Running)
		{
			m_MainWindow->PollEvents();

			const double currentFrame = glfwGetTime();
			const double m_GameTime = currentFrame - m_LastFrameTime;


			if (!RenderCommand::IsGraphicsBackendReady())
			{
				continue;
			}

			RenderCommand::AcquireImageIndex();

			Imgui::Begin();
			for (const auto layer : m_Layers)
			{
				layer->OnImguiRender();
			}
			
			Imgui::End();

			for (const auto layer : m_Layers)
			{
				layer->OnUpdate(static_cast<float>(m_GameTime));
			}


			RenderCommand::BeginFrameCommands(m_MainSwapChainCommandBuffer);
			RenderCommand::BeginRenderPass(RenderCommand::GetSwapChain());

			RenderCommand::Clear(ClearSettings{ {0.2, 0.2, 0.2, 1.0f} });
			
			Imgui::SubmitToCommandBuffer(RenderCommand::GetCurrentCommandBuffer());
			
			RenderCommand::EndRenderPass();
			RenderCommand::EndFrameCommands();


			RenderCommand::WaitAndRender();
			RenderCommand::PresentImage();


			m_LastFrameTime = currentFrame;
		}

	}
}
