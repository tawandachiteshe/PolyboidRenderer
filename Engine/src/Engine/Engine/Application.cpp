﻿#include "boidpch.h"


#include "Application.h"
#include <spdlog/spdlog.h>

#include "Engine.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/Swapchain.h"

#include "ImguiSetup.h"
#include "Engine/Renderer/Renderer2D.h"
#include "EntryPoint.h"
#include "imgui.h"
#include "imgui_impl_vulkan.h"
#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/CommandList.h"
#include "Engine/Renderer/PipelineDescriptorSet.h"
#include "Engine/Renderer/PipelineDescriptorSetPool.h"
#include "Engine/Renderer/PipelineState.h"
#include "Engine/Renderer/RendererSyncObjects.h"
#include "Engine/Renderer/RenderPass.h"
#include "Engine/Renderer/UniformBuffer.h"
#include "Engine/Renderer/VertexBufferArray.h"
#include "Events/EventDispatcher.h"
#include "Events/WindowEvent.h"
#include "GLFW/glfw3.h"
#include "Platform/Vulkan/VkRenderAPI.h"
#include "Platform/Vulkan/VkSwapChain.h"
#include "Platform/Vulkan/VulkanRenderPass.h"
#include "Platform/Vulkan/Utils/VulkanDevice.h"
#include "Registry/ShaderRegistry.h"


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

		Renderer::Init(m_RenderAPI, m_Settings);
		ShaderRegistry::Init(m_RenderAPI);
		//Imgui::Init(m_MainWindow->GetNativeWindow());
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
		//Renderer::Resize();

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
		delete m_RenderAPI;
	}

	void Application::Run()
	{
		OPTICK_THREAD("Main Thread")

		Engine::Init();

		m_CommandList = CommandList::Create({3});
		auto mainSyncObjects = RendererSyncObjects::Create(3);


		const auto skyboxShaders = ShaderRegistry::LoadGraphicsShaders("Renderer3D/triangle");

		struct Vertex
		{
			glm::vec3 pos;
			glm::vec4 norm;
			glm::vec2 uv;
		};

		Vertex vert[] = {
			{{-1.0f, 1.0f, 0.0f}, {1.f, 1.f, 0.0f, 1.0f}, {0.0f, 0.0f}},
			{{-1.0f, -1.0f, 0.0f}, {1.f, 1.f, 0.0f, 1.0f}, {0.0f, 1.0f}},
			{{1.0f, -1.0f, 0.0f}, {1.f, 1.f, 0.0f, 1.0f}, {1.0f, 1.0f}},
			{{1.0f, 1.0f, 0.0f}, {1.f, 1.f, 0.0f, 1.0f}, {1.0f, 0.0f}}
		};

		unsigned int indices[] = {
			0, 1, 3, // Triangle 1
			1, 2, 3 // Triangle 2
		};

		auto triVerts = VertexBuffer::Create(vert, sizeof(vert));
		triVerts->SetLayout({
			{ShaderDataType::Float3, "aPosition"},
			{ShaderDataType::Float4, "aNormal"},
			{ShaderDataType::Float2, "aUV"}
		});
		auto triIdxs = IndexBuffer::Create(indices, 6);
		auto vtxArray = VertexBufferArray::Create();
		vtxArray->AddVertexBuffer(triVerts);
		vtxArray->SetIndexBuffer(triIdxs);

		const uint32_t green = 0xFF'00'FF'00;

		auto greenTexture = Texture::Create({
			                                    .sizedFormat = EngineGraphicsFormats::RGBA8,
			                                    .usage = ImageUsage::Sampling, .Width = 1, .Height = 1
		                                    }, &green);


		auto pool = PipelineDescriptorSetPool::Create({3});

		auto skyBoxPipeline = PipelineState::CreateGraphicsPipeline();
		skyBoxPipeline->SetGraphicsShaders(skyboxShaders);
		//skyBoxPipeline->SetVertexArray(vtxArray);
		skyBoxPipeline->SetRenderPass(Renderer::GetSwapChain()->GetDefaultRenderPass());
		skyBoxPipeline->Bake();

		m_Pipeline = skyBoxPipeline;

		// skyBoxPipeline->AllocateDescSetsFromShaders(pool);
		// auto descSets = skyBoxPipeline->GetDescriptorSets(0);

		//ShaderRegistry::Exist("Renderer3D/skybox");
		//ShaderRegistry::LoadGraphicsShaders("");

		struct CameraBufferData
		{
			glm::mat4 projection;
			glm::mat4 view;
		};

		std::vector<Ref<UniformBuffer>> uniformBuffers;
		std::vector<Ref<StorageBuffer>> storageBuffers;
		//
		// for (int i = 0; i < 3; ++i)
		// {
		// 	auto cameraDataUB = UniformBuffer::Create(sizeof(CameraBufferData));
		// 	descSets[i]->WriteUniformBuffer(0, cameraDataUB);
		// 	uniformBuffers.emplace_back(cameraDataUB);
		//
		// 	auto storageBuffer = StorageBuffer::Create(sizeof(vert));
		// 	descSets[i]->WriteStorageBuffer(1, storageBuffer);
		// 	storageBuffers.emplace_back(storageBuffer);
		//
		// 	descSets[i]->WriteTexture2D(2, greenTexture);
		//
		//
		// 	descSets[i]->Commit();
		// }

		/*
		for (int i = 0; i < 3; ++i)
		{

			Renderer::BeginCommandBuffer(m_CommandList->GetCommandBufferAt(i));
			auto swapchain = Renderer::GetSwapChain();
			Renderer::BeginRenderPass(swapchain->GetDefaultRenderPass(), swapchain->GetFrameBuffers().at(i));

			Renderer::Clear(glm::vec4(0.2, 0.2, 0.2, 1));

			//
			// Imgui::Begin(mainCmdList);
			//
			// ImGui::Begin("Tawanda");
			//
			// ImGui::End();
			//
			// Imgui::End();

			Renderer::BindGraphicsPipeline(skyBoxPipeline);




			Viewport viewport{};
			viewport.Width = 1600;
			viewport.Height = 900;
			viewport.MinDepth = 0.0;
			viewport.MaxDepth = 1.0f;


			Renderer::SetViewport(viewport);
			Rect rect{};
			rect.Width = 1600;
			rect.Height = 900;
			Renderer::SetScissor(rect);


			// Renderer::BindVertexBuffer(triVerts);
			// Renderer::BindIndexBuffer(triIdxs);
			//Renderer::BindGraphicsDescriptorSets(0, descSets);
			//Renderer::DrawIndexed(6);
			


			Renderer::EndRenderPass();
			Renderer::EndCommandBuffer(m_CommandList->GetCommandBufferAt(i));

			
		}
		*/


		while (m_Running)
		{
			OPTICK_FRAME("Main Frame")

			m_MainWindow->PollEvents();

			const double currentFrame = glfwGetTime();
			double m_GameTime = currentFrame - m_LastFrameTime;
			m_LastFrameTime = currentFrame;


			// update here.....
			for (auto layer : m_Layers)
			{
				layer->OnUpdate(static_cast<float>(m_GameTime));
			}

			//Note these commands are executed later
			Renderer::BeginFrame(mainSyncObjects);
			Renderer::BeginCommands(m_CommandList);
			Renderer::BeginSwapChainRenderPass();
			Renderer::Clear(ClearSettings{ { 0.2, 0.2, 0.2, 1.0f } });

			Renderer::BindGraphicsPipeline(skyBoxPipeline);


			Viewport viewport{};
			viewport.Width = 1600;
			viewport.Height = 900;
			viewport.MinDepth = 0.0;
			viewport.MaxDepth = 1.0f;


			Renderer::SetViewport(viewport);
			Rect rect{};
			rect.Width = 1600;
			rect.Height = 900;
			Renderer::SetScissor(rect);

			Renderer::DrawArrays(12 * 3);

			Renderer::EndSwapChainRenderPass();
			Renderer::EndCommands();

			Renderer::SubmitCommandList(m_CommandList);
			Renderer::EndFrame();


			m_MainWindow->PollEvents();
		}
	}
}
