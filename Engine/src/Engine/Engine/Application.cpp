#include "boidpch.h"


//#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "Application.h"
#include <spdlog/spdlog.h>

#include "Engine.h"
#include "Engine/Renderer/RenderCommand.h"
#include "Engine/Renderer/Swapchain.h"

#include "ImguiSetup.h"
#include "Engine/Renderer/Renderer2D.h"
#include "EntryPoint.h"
#include "imgui.h"
#include "imgui_impl_vulkan.h"
#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/BufferSet.h"
#include "Engine/Renderer/CommandBufferSet.h"
#include "Engine/Renderer/PipelineDescriptorSet.h"
#include "Engine/Renderer/PipelineDescriptorSetPool.h"
#include "Engine/Renderer/GraphicsPipeline.h"
#include "Engine/Renderer/KomputeCommand.h"
#include "Engine/Renderer/RenderPass.h"
#include "Engine/Renderer/UniformBuffer.h"
#include "Engine/Renderer/VertexBufferArray.h"
#include "Events/EventDispatcher.h"
#include "Events/WindowEvent.h"
#include "GLFW/glfw3.h"
#include "Platform/Vulkan/VulkanGraphicsPipeline.h"
#include "Registry/ShaderRegistry.h"
#include "Utils/SmartPtr.h"


namespace Polyboid
{
	class VkRenderAPI;
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

		RenderCommand::Init(m_RenderAPI, m_Settings);
		KomputeCommand::Init();
		ShaderRegistry::Init(m_RenderAPI);
		Imgui::Init(m_MainWindow->GetNativeWindow());
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
		EngineMemoryManager::FreeMem(m_RenderAPI);
		spdlog::info("Allocation Count: {}", EngineMemoryManager::GetAllocationCount());
		spdlog::info("Free Count: {}", EngineMemoryManager::GetFreeCount());
		spdlog::warn("Memory not freed {}",
		             EngineMemoryManager::GetAllocationCount() - EngineMemoryManager::GetFreeCount());
	}

	void Application::Run()
	{
		OPTICK_THREAD("Main Thread")

		Engine::Init();

		m_CommandList = CommandBufferSet::Create({3, CommandType::ManyTime});


		RenderCommand::PushCommandBufferSets({ m_CommandList });


		static float rotation = 0;

		while (m_Running)
		{
			m_MainWindow->PollEvents();

			const double currentFrame = glfwGetTime();
			double m_GameTime = currentFrame - m_LastFrameTime;
			m_LastFrameTime = currentFrame;

			if (!RenderCommand::IsGraphicsBackendReady())
			{
				continue;
			}

			KomputeCommand::WaitForWork();
			RenderCommand::AcquireImageIndex();

			for (auto layer : m_Layers)
			{
				layer->OnUpdate(static_cast<float>(m_GameTime));
			}



			Imgui::Begin();
			for (auto layer : m_Layers)
			{
				layer->OnImguiRender();
			}

			Imgui::End();


			RenderCommand::BeginFrameCommands(m_CommandList);
			RenderCommand::BeginRenderPass(RenderCommand::GetSwapChain());
			RenderCommand::Clear(ClearSettings{{0.2, 0.2, 0.2, 1.0f}});

			Imgui::SubmitToCommandBuffer(RenderCommand::GetCurrentCommandBuffer());

			RenderCommand::EndRenderPass();
			RenderCommand::EndFrameCommands();

			KomputeCommand::WaitAndCompute();
			RenderCommand::WaitAndRender();
		}

		int a = 2000;
	}
}
