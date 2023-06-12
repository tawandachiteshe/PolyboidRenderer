#include "ViewportWindow.h"
#include <imgui.h>
#include <spdlog/spdlog.h>

#include "Editor/EditorCamera.h"
#include "Engine/Engine/Events/EventDispatcher.h"
#include "Engine/Engine/Engine.h"
#include "Engine/Engine/ImguiSetup.h"
#include "Engine/Engine/Registry/ShaderRegistry.h"
#include "Engine/Renderer/GraphicsPipeline.h"
#include "Engine/Renderer/KomputeCommand.h"
#include "Engine/Renderer/KomputePipeline.h"
#include "Engine/Renderer/RenderCommand.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Renderer/VertexBufferArray.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Polyboid
{
#define GetName(x) #x

	ViewportWindow::ViewportWindow(const std::string& name)
	{
		m_Name = name;

		float fov = 45.0f;
		m_ViewportCamera = CreateRef<EditorCamera>(fov, 1.777, 0.1f, 2000.0f);

		TextureSettings textureSettings = { .sizedFormat = EngineGraphicsFormats::RGBA8, .usage = ImageUsage::Sampling, .path = "Assets/Textures/pic.jpg" };
		textureSettings.mipCount = 10;
		textureSettings.generateMips = true;

		auto texture = Texture2D::Create(textureSettings);

		ImageSettings imageSettings{};
		imageSettings.height = 400;
		imageSettings.width = 400;
		imageSettings.format = EngineGraphicsFormats::RGBA8;


		auto image2d = Image2D::Create(imageSettings);

		m_EditorCommandBuffer = CommandBufferSet::Create({3, CommandType::ManyTime});
		m_KomputeCommandBuffer = CommandBufferSet::Create({3, CommandType::ManyTime});
		m_AgeBuffer = StorageBufferSet::Create(sizeof(uint32_t) * 100);

		m_RefComputePipeline = KomputePipeline::Create();
		m_RefComputePipeline->SetComputeShader(ShaderRegistry::Load("Renderer3D/Compute/testKompute.comp"));
		m_RefComputePipeline->Bake();

		m_RefComputePipeline->AllocateDescriptorSets(0);
		m_RefComputePipeline->BindStorageBufferSet(0, m_AgeBuffer, 0);
		m_RefComputePipeline->BindImage2D(1, image2d, 0);
		m_RefComputePipeline->WriteSetResourceBindings(0);

		KomputeCommand::PushCommandBufferSet(m_KomputeCommandBuffer);

		

		for (uint32_t i = 0; i < 3; ++i)
		{
			KomputeCommand::BeginCommands(m_KomputeCommandBuffer, i);
			KomputeCommand::BindKomputePipeline(m_RefComputePipeline);
			KomputeCommand::BindDescriptorSet(m_RefComputePipeline->GetDescriptorSets(0));
			KomputeCommand::Dispatch({400, 400, 1});
			KomputeCommand::EndCommands();
			KomputeCommand::ComputeOneTime({m_KomputeCommandBuffer}, i);
		}


		const auto skyboxShaders = ShaderRegistry::LoadGraphicsShaders("Renderer3D/skybox");
		RenderPassSettings renderPassSettings{};
		renderPassSettings.Height = 600;
		renderPassSettings.Width = 800;
		renderPassSettings.TextureAttachments = {{TextureAttachmentSlot::Color0, EngineGraphicsFormats::RGBA8}};
		renderPassSettings.debugName = "Offscreen render pass";

		m_RenderPass = RenderPass::Create(renderPassSettings);
		m_FrameBuffers = FrameBufferSet::Create(m_RenderPass);


		m_Vertices[0] = {{-1.0f, 1.0f, 0.0f}, {1.f, 1.f, 0.0f, 1.0f}, {0.0f, 0.0f}};
		m_Vertices[1] = {{-1.0f, -1.0f, 0.0f}, {1.f, 1.f, 0.0f, 1.0f}, {0.0f, 1.0f}};
		m_Vertices[2] = {{1.0f, -1.0f, 0.0f}, {1.f, 1.f, 0.0f, 1.0f}, {1.0f, 1.0f}};
		m_Vertices[3] = {{1.0f, 1.0f, 0.0f}, {1.f, 1.f, 0.0f, 1.0f}, {1.0f, 0.0f}};


		unsigned int indices[] = {
			0, 1, 3, // Triangle 1
			1, 2, 3 // Triangle 2
		};

		m_VertexBuffer = VertexBufferSet::Create(sizeof(m_Vertices));
		m_VertexBuffer->SetLayout({
			{ShaderDataType::Float3, "aPosition"},
			{ShaderDataType::Float4, "aNormal"},
			{ShaderDataType::Float2, "aUV"}
		});
		m_IndexBuffer = IndexBuffer::Create(indices, 6);
		auto vtxArray = VertexBufferArray::Create();
		vtxArray->AddVertexBufferSet(m_VertexBuffer);
		vtxArray->SetIndexBuffer(m_IndexBuffer);

		const uint32_t green = 0xFF'00'FF'00;

		const uint32_t greenRed[] = {
			0xFF'00'FF'00, 0xFF'FF'FF'00, 0xFF'00'00'FF, 0xFF'FF'00'45,
			0xFF'00'FF'FF, 0xFF'FF'FF'00, 0xFF'00'00'FF, 0xFF'FF'00'45,
			0xFF'00'FF'FF, 0xFF'FF'FF'00, 0xFF'00'00'FF, 0xFF'FF'00'45,
			0xFF'00'FF'FF, 0xFF'FF'FF'00, 0xFF'00'00'FF, 0xFF'FF'00'45,
			0xFF'00'FF'FF, 0xFF'FF'FF'00, 0xFF'00'00'FF, 0xFF'FF'00'45,
			0xFF'00'FF'FF, 0xFF'FF'FF'00, 0xFF'00'00'FF, 0xFF'FF'00'45
		};

		auto greenTexture = Texture2D::Create({
			                                    .sizedFormat = EngineGraphicsFormats::RGBA8,
			                                    .usage = ImageUsage::Sampling, .Width = 1, .Height = 1,

		                                    }, &green);

		TextureSettings greenTexture3dSettings{ .sizedFormat = EngineGraphicsFormats::RGBA8,
			.usage = ImageUsage::Sampling, .Width = 2, .Height = 2 };

		greenTexture3dSettings.generateMips = true;
		greenTexture3dSettings.mipCount = 2;

		auto greenTexture3D = Texture3D::Create(greenTexture3dSettings, greenRed);

		auto checkerTexture = Texture2D::Create({
			.sizedFormat = EngineGraphicsFormats::RGBA8, .usage = ImageUsage::Sampling,
			.path = "Assets/Textures/checker.jpg"
		});


		m_Pipeline = GraphicsPipeline::Create();
		m_Pipeline->SetGraphicsShaders(skyboxShaders);
		m_Pipeline->SetVertexArray(vtxArray);
		m_Pipeline->SetRenderPass(m_RenderPass);
		m_Pipeline->Bake();


		//ShaderRegistry::Exist("Renderer3D/skybox");
		//ShaderRegistry::LoadGraphicsShaders("");


		m_UniformBuffers = UniformBufferSet::Create(sizeof(CameraBufferData));
		m_StorageBuffers = StorageBufferSet::Create(sizeof(m_Vertices));
		m_UniformStagingBuffers = StagingBufferSet::Create(sizeof(CameraBufferData));
		m_StorageStagingBuffers = StagingBufferSet::Create(sizeof(m_Vertices));
		m_StorageStagingBuffersVB = StagingBufferSet::Create(sizeof(m_Vertices));

		m_Pipeline->AllocateDescriptorSets();
		m_Pipeline->BindUniformBufferSet(0, m_UniformBuffers);
		m_Pipeline->BindStorageBufferSet(1, m_StorageBuffers);
		m_Pipeline->BindTexture2D(2, texture);
		m_Pipeline->BindStorageBufferSet(3, m_AgeBuffer);
		m_Pipeline->BindTexture3D(4, greenTexture3D);
		m_Pipeline->WriteSetResourceBindings();
		//

		m_CameraData.projection = glm::perspective(1600.f / 900.f, glm::radians(45.0f), 0.01f, 1000.0f);
		m_CameraData.view = glm::translate(glm::mat4(1.0f), {0.0f, 0.0f, -2.0f});


		for (uint32_t i = 0; i < RenderCommand::GetMaxFramesInFlight(); ++i)
		{
			m_FramebufferTextures.push_back(
				Imgui::GetVulkanTextureID(m_FrameBuffers->Get(i)->GetColorAttachment(TextureAttachmentSlot::Color0)));
		}

		// RenderCommand::RegisterFreeFunc([&]
		// {
		// 	spdlog::info("Freeing Resources");
		//
		//
		// 	for (uint32_t i = 0; i < RenderCommand::GetMaxFramesInFlight(); ++i)
		// 	{
		// 		Imgui::FreeVulkanTextureID(m_FramebufferTextures.at(i));
		// 	}
		//
		// 	m_FramebufferTextures.clear();
		//
		// 	for (uint32_t i = 0; i < RenderCommand::GetMaxFramesInFlight(); ++i)
		// 	{
		// 		m_FramebufferTextures.push_back(
		// 			Imgui::GetVulkanTextureID(
		// 				m_FrameBuffers->Get(i)->GetColorAttachment(TextureAttachmentSlot::Color0)));
		// 	}
		// });

		RenderCommand::PushCommandBufferSets({m_EditorCommandBuffer});

		Renderer2D::Init(m_RenderPass);
	}

	ViewportWindow::~ViewportWindow()
	{
	}


	void ViewportWindow::OnGameObjectSelected(const Event& event)
	{
		m_CurrentGameObject = nullptr;
	}

	void ViewportWindow::OnGameObjectDeleted(const Event& event)
	{
		m_CurrentGameObject = nullptr;
	}

	static CameraBufferData lod{};

	void ViewportWindow::RenderImgui()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});
		ImGui::Begin(m_Name.c_str());
		const auto mainRenderTarget = RenderCommand::GetDefaultRenderTarget();
		const auto windowSize = ImGui::GetContentRegionAvail();

		ImGui::Image(m_FramebufferTextures.at(RenderCommand::GetCurrentFrame()), windowSize);

		ImGui::End();

		ImGui::PopStyleVar();

		ImGui::Begin("Lod controller");
		ImGui::SliderFloat("Texture lod", &lod.projection[0][0], 0.0, 10.0f);
		ImGui::End();
	}

	void ViewportWindow::Update(float ts)
	{
		static float dt = 0.0;
		if (dt > 0.01f)
		{
			m_ViewportCamera->OnUpdate(ts);
			m_ViewportCamera->SetEnableInput(true);
			dt = 0;
		}
		else
		{
			dt += ts;
		}

		m_CameraData.view = m_ViewportCamera->GetViewMatrix();
		m_CameraData.projection = m_ViewportCamera->GetProjection();

		m_Rotation += 100 * dt;

		m_EntityBufferData.transform = glm::mat4(1.0f) * glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation),
		                                                             {0, 0.0f, 1.0}) * glm::scale(
			glm::mat4(1.0f), {0.2, 0.2, 0.2});

		m_EntityBufferData2.transform = glm::translate(glm::mat4(1.0f), { 0.0, 0.5f, 0.0f });

		KomputeCommand::BeginFrameCommands(m_KomputeCommandBuffer);
		KomputeCommand::BindKomputePipeline(m_RefComputePipeline);
		KomputeCommand::BindDescriptorSet(m_RefComputePipeline->GetDescriptorSets(0));
		KomputeCommand::Dispatch({ 100, 1, 1 });
		KomputeCommand::EndFrameCommands();

		RenderCommand::BeginFrameCommands(m_EditorCommandBuffer);


		RenderCommand::BeginRenderPass(m_RenderPass, m_FrameBuffers);
		RenderCommand::BindGraphicsPipeline(m_Pipeline);
		RenderCommand::BindGraphicsDescriptorSets(0, m_Pipeline->GetDescriptorSets(0));
		RenderCommand::VertexShaderPushConstants(m_Pipeline, &m_EntityBufferData, sizeof(m_EntityBufferData));
		RenderCommand::FragmentShaderPushConstants(m_Pipeline, &lod, sizeof(lod));

		Viewport viewport{};
		viewport.Width = 800;
		viewport.Height = 600;
		viewport.MinDepth = 0.0;
		viewport.MaxDepth = 1.0f;

		RenderCommand::SetViewport(viewport);
		Rect rect{};
		rect.Width = 800;
		rect.Height = 600;
		RenderCommand::SetScissor(rect);
		RenderCommand::BindVertexBuffer(m_VertexBuffer);
		RenderCommand::BindIndexBuffer(m_IndexBuffer);
		RenderCommand::DrawIndexed(6);
		RenderCommand::VertexShaderPushConstants(m_Pipeline, &m_EntityBufferData2, sizeof(m_EntityBufferData2));
		RenderCommand::DrawIndexed(6);

		OnRender();

		RenderCommand::EndRenderPass();

		RenderCommand::SetStagingBufferData(m_UniformStagingBuffers, &m_CameraData);
		RenderCommand::SetStagingBufferData(m_StorageStagingBuffers, m_Vertices);
		RenderCommand::SetStagingBufferData(m_StorageStagingBuffersVB, m_Vertices);
		RenderCommand::CopyStagingBuffer(m_UniformStagingBuffers, m_UniformBuffers);
		RenderCommand::CopyStagingBuffer(m_StorageStagingBuffers, m_StorageBuffers);
		RenderCommand::CopyStagingBuffer(m_StorageStagingBuffersVB, m_VertexBuffer);

		Renderer2D::UploadDataToGpu();

		RenderCommand::EndFrameCommands();
	}

	void ViewportWindow::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Bind<MouseScrollEvent>(BIND_EVENT(m_ViewportCamera->OnMouseScroll));
	}

	void ViewportWindow::OnRender()
	{
		//spdlog::info("Mouse x: {} y: {}", Input::GetMouseX(), Input::GetMouseY());

		Renderer2D::BeginDraw(m_ViewportCamera);

		glm::mat4 pos = glm::translate(glm::mat4(1.0f), {0.0, 1.5f, 0.0});
		glm::mat4 pos2 = glm::translate(glm::mat4(1.0f), {0.0, 2.5f, 0.0});
		glm::mat4 pos3 = glm::translate(glm::mat4(1.0f), {0.0, 3.5f, 0.0});

		static float rotation = 0.0f;
		rotation += 10 * 0.01;


		Renderer2D::DrawLine({0.0, 0.0, 0.0}, {2.0f, 0.0, 0.0});
		Renderer2D::DrawCube(pos2);
		Renderer2D::DrawCube(pos3);
		Renderer2D::DrawCircle(pos);
		Renderer2D::DrawQuad(glm::mat4(1.0f), glm::vec4{1.2, 0.2, 0.2, 1.0f});
		Renderer2D::DrawQuad(pos2, glm::vec4{1.2, 1.2, 0.2, 1.0f});
		Renderer2D::DrawRotatedQuad({0.0, 2.5f, 0.0}, rotation);


		Renderer2D::EndDraw();
	}
}
