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
#include "Engine/Renderer/Material.h"
#include "Engine/Renderer/RenderCommand.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Renderer/Renderer3D.h"
#include "Engine/Renderer/TexelBuffers.h"
#include "Engine/Renderer/VertexBufferArray.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Polyboid
{
	struct Buffer
	{
		uint8_t* m_Data = nullptr;
		size_t m_Size = 0;
		Buffer() = default;

		Buffer(size_t size): m_Size(size)
		{
			m_Data = new uint8_t[size];
			std::memset(m_Data, 23, size);
		}

		Buffer(const Buffer& other)
			: m_Data(other.m_Data),
			  m_Size(other.m_Size)
		{
			std::memcpy(m_Data, other.m_Data, m_Size);
			std::memset(m_Data, 1, m_Size);
		}

		Buffer(Buffer&& other) noexcept
			: m_Data(other.m_Data),
			  m_Size(other.m_Size)
		{
		}

		Buffer& operator=(const Buffer& other)
		{
			if (this == &other)
				return *this;
			m_Data = new uint8_t[other.m_Size];
			std::memcpy(m_Data, other.m_Data, other.m_Size);
			std::memset(m_Data, 1, other.m_Size);
			m_Size = other.m_Size;
			return *this;
		}

		Buffer& operator=(Buffer&& other) noexcept
		{
			if (this == &other)
				return *this;
			std::memmove(m_Data, other.m_Data, other.m_Size);
			std::memset(m_Data, 1, other.m_Size);
			m_Size = other.m_Size;
			return *this;
		}

		~Buffer()
		{
			delete m_Data;
		}
	};


	ViewportWindow::ViewportWindow(const std::string& name)
	{
		m_Name = name;

		float fov = 45.0f;
		m_ViewportCamera = CreateRef<EditorCamera>(fov, 1.777, 0.1f, 2000.0f);

		m_Vertices[0] = {{-1.0f, 1.0f, 0.0f}, {1.f, 1.f, 0.0f, 1.0f}, {0.0f, 0.0f}};
		m_Vertices[1] = {{-1.0f, -1.0f, 0.0f}, {1.f, 1.f, 0.0f, 1.0f}, {0.0f, 1.0f}};
		m_Vertices[2] = {{1.0f, -1.0f, 0.0f}, {1.f, 1.f, 0.0f, 1.0f}, {1.0f, 1.0f}};
		m_Vertices[3] = {{1.0f, 1.0f, 0.0f}, {1.f, 1.f, 0.0f, 1.0f}, {1.0f, 0.0f}};


		unsigned int indices[] = {
			0, 1, 3, // Triangle 1
			1, 2, 3 // Triangle 2
		};


		m_VertexBuffer = VertexBuffer::Create(m_Vertices, sizeof(m_Vertices));
		m_IndexBuffer = IndexBuffer::Create(indices, 6);

		m_VertexBuffer2 = VertexBuffer::Create(m_Vertices, sizeof(m_Vertices));
		m_IndexBuffer2 = IndexBuffer::Create(indices, 3);

		m_RedMaterial = Renderer3D::CreateMaterial("Red");
		m_BlueMaterial = Renderer3D::CreateMaterial("Blue");

		for (uint32_t i = 0; i < RenderCommand::GetMaxFramesInFlight(); ++i)
		{
			m_FramebufferTextures.push_back(
				Imgui::CreateVulkanTextureID(Renderer3D::GetCompositeTexture(TextureAttachmentSlot::Color0, i)));
		}


	
	}



	void ViewportWindow::OnGameObjectSelected(const Event& event)
	{
		m_CurrentGameObject = nullptr;
	}

	void ViewportWindow::OnGameObjectDeleted(const Event& event)
	{
		m_CurrentGameObject = nullptr;
	}

	void ViewportWindow::ResizeViewportFrameBuffers(const glm::uvec2& size)
	{
		for (uint32_t i = 0; i < 3; ++i)
		{
			Imgui::FreeVulkanTextureID(m_FramebufferTextures.at(i));
			m_FramebufferTextures[i] = Imgui::CreateVulkanTextureID(
				Renderer3D::GetCompositeTexture(TextureAttachmentSlot::Color0, i));
		}
	}

	ImTextureID ViewportWindow::GetImGuiCompositeTexture() const
	{
		return m_FramebufferTextures.at(RenderCommand::GetCurrentFrame());
	}

	static CameraBufferData lod{};

	void ViewportWindow::RenderImgui()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});
		ImGui::Begin(m_Name.c_str());

		const auto imguiWindowSize = ImGui::GetContentRegionAvail();
		const auto windowSize = glm::uvec2(static_cast<uint32_t>(imguiWindowSize.x),
		                                   static_cast<uint32_t>(imguiWindowSize.y));

		if (windowSize.x > 0 && windowSize.y > 0 && (windowSize.x != static_cast<uint32_t>(m_LastViewportSize.x) || windowSize.y !=
			static_cast<uint32_t>(m_LastViewportSize.y)))
		{
			

			spdlog::info("On Viewport window resize {}.x {}.y", windowSize.x, windowSize.y);
			m_LastViewportSize = {windowSize.x, windowSize.y};
	
			Renderer3D::ReSize(m_LastViewportSize);
			ResizeViewportFrameBuffers(m_LastViewportSize);
		}


		ImGui::Image(GetImGuiCompositeTexture(),{m_LastViewportSize.x, m_LastViewportSize.y});
		ImGui::End();
		ImGui::PopStyleVar();

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


		const auto tranform = glm::mat4(1.0f) * glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation),
		                                                             {0, 0.0f, 1.0}) * glm::scale(
			glm::mat4(1.0f), {3.2, 3.2, 3.2});

		const auto tranform2 = glm::mat4(1.0f) * glm::translate(glm::mat4(1.0f), glm::vec3(0, 2.0f, -2));

		Renderer3D::BeginScene(m_ViewportCamera);

		Renderer3D::Clear(glm::vec4{ .2f });
		Renderer3D::SetViewport(m_LastViewportSize);
		m_RedMaterial->SetColor({ 1, 0, 0 });
		Renderer3D::DrawMesh(m_VertexBuffer, m_IndexBuffer, tranform, m_RedMaterial);
		m_BlueMaterial->SetColor({ 0, 1, 0 });
		Renderer3D::DrawMesh(m_VertexBuffer2, m_IndexBuffer2, tranform2, m_BlueMaterial);

		OnRender();
		Renderer3D::EndScene();


	}

	void ViewportWindow::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Bind<MouseScrollEvent>(BIND_EVENT(m_ViewportCamera->OnMouseScroll));
	}

	void ViewportWindow::OnRender()
	{
		//spdlog::info("Mouse x: {} y: {}", Input::GetMouseX(), Input::GetMouseY());


		glm::mat4 pos = glm::translate(glm::mat4(1.0f), {0.0, 1.5f, 0.0});
		glm::mat4 pos2 = glm::translate(glm::mat4(1.0f), {0.0, 2.5f, 0.0});
		glm::mat4 pos3 = glm::translate(glm::mat4(1.0f), {0.0, 3.5f, 0.0});

		static float rotation = 0.0f;
		rotation += 10.0f * 0.01f;


		Renderer2D::DrawLine({0.0, 0.0, 0.0}, {2.0f, 0.0, 0.0});
		Renderer2D::DrawCube(pos2);
		Renderer2D::DrawCube(pos3);
		Renderer2D::DrawCube(pos3);
		Renderer2D::DrawCircle(pos);
		Renderer2D::DrawQuad(glm::mat4(1.0f), glm::vec4{1.2, 0.2, 0.2, 1.0f});
		Renderer2D::DrawQuad(pos2, glm::vec4{1.2, 1.2, 0.2, 1.0f});
		Renderer2D::DrawRotatedQuad({0.0, 2.5f, 0.0}, rotation);


	}
}
