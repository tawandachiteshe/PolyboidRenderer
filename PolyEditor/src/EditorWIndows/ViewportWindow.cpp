#include "ViewportWindow.h"
#include <imgui.h>
#include <spdlog/spdlog.h>

#include "Editor/EditorCamera.h"
#include "Engine/Engine/Events/EventDispatcher.h"
#include "Engine/Engine/Engine.h"
#include "Engine/Engine/Input.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Renderer/RenderPass.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Polyboid
{


#define GetName(x) #x

	ViewportWindow::ViewportWindow(const std::string& name)
	{
		m_Name = name;
		
		float fov = 45.0f;
		m_ViewportCamera = std::make_shared<EditorCamera>(fov, 1.777, 0.1f, 2000.0f);
		
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

	void ViewportWindow::RenderImgui()
	{

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
		ImGui::Begin(m_Name.c_str());
		const auto mainRenderTarget = Renderer::GetDefaultRenderTarget();
		const auto colorTexture = mainRenderTarget->GetTexture(TextureAttachmentSlot::Color0);
		const auto windowSize = ImGui::GetContentRegionAvail();

		//ImGui::Image(std::any_cast<ImTextureID>(colorTexture->GetHandle()), windowSize, {1.0f, 1.0f}, {0.0f, 0.0f});

		ImGui::End();

		ImGui::PopStyleVar();

	}

	void ViewportWindow::Update(float ts)
	{
		static  float dt = 0.0;
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

		glm::mat4 pos = glm::translate(glm::mat4(1.0f), { 0.0, 1.5f, 0.0 });
		glm::mat4 pos2 = glm::translate(glm::mat4(1.0f), { 0.0, 2.5f, 0.0 });



		Renderer2D::DrawLine({0.0, 0.0, 0.0}, {2.0f, 0.0, 0.0});
		Renderer2D::DrawCube(pos2);
		Renderer2D::DrawCircle(pos);
		Renderer2D::DrawQuad(glm::mat4(1.0f), glm::vec4{1.2, 0.2, 0.2, 1.0f});
		

		Renderer2D::EndDraw();
	}
}

