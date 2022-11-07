#include "ViewportWindow.h"
#include <imgui.h>
#include <spdlog/spdlog.h>

#include "Engine/Engine/Application.h"
#include "Editor/EditorCamera.h"
#include "Editor/Events/EditorEvents.h"
#include "Engine/Engine/Input.h"
#include "Engine/Engine/ECS/ECSManager.h"
#include "Engine/Engine/ECS/GameObject.h"
#include "Engine/Engine/Events/EventSystem.h"
#include "Engine/Engine/Gameplay/GameInstance.h"
#include "Engine/Engine/Gameplay/World.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/Renderer2D.h"
#include "glm/gtc/type_ptr.hpp"
#include "Engine/Engine/Math/Math.h"


namespace Polyboid
{


	ViewportWindow::ViewportWindow(const std::string& name)
	{
		m_Name = name;

		auto app = Application::Get();
		auto& appdata = app->GetAppData();
		auto aspect = appdata.windowSpecs.GetAspectRatio();

		float fov = 45.0f;


		m_ViewportCamera = std::make_shared<EditorCamera>(fov, aspect, 0.1f, 1000.0f);
		m_Framebuffer = Framebuffer::MakeFramebuffer({ appdata.windowSpecs.Width, appdata.windowSpecs.Height });

		GameInstance::SetCurrentCamera(m_ViewportCamera);

		EventSystem::Bind(EventType::ON_GAME_OBJECT_SELECTED, BIND_EVENT(OnGameObjectSelected));
	}

	ViewportWindow::~ViewportWindow()
	{
	}

	static bool DidWindowSizeChange(const ImVec2& a, const ImVec2& b)
	{
		auto xDiff = b.x - a.x;
		auto yDiff = b.y - a.y;

		return yDiff || xDiff;
	}

	static bool boolIsNotZero(const ImVec2& v)
	{
		return (v.x && v.y);
	}

	void ViewportWindow::OnGameObjectSelected(const Event& event)
	{

		auto gameEvent = CastEventAs<GameObjectOutlineClick>(event);
		m_CurrentGameObject = gameEvent.GetGameObjectID();

		spdlog::info("Game Selected event {}", m_CurrentGameObject);

	}

	void ViewportWindow::RenderImgui()
	{

		
		ImGui::Begin(m_Name.c_str());

		 ImGui::BeginChild("GameRender");

		 auto hovered = ImGui::IsWindowHovered();
		 m_Focused = ImGui::IsWindowFocused();
		 ImGui::SetNextFrameWantCaptureMouse(true);

		 
		// spdlog::info("hovered {0} , focused {1}", hovered, m_Focused);


		 auto contentSize = ImGui::GetWindowContentRegionMax();

		if (boolIsNotZero(contentSize) && DidWindowSizeChange(m_LastViewportWindowSize, contentSize))
		{
			m_Framebuffer->Resize(contentSize.x, contentSize.y);
			m_ViewportCamera->SetViewportSize(contentSize.x, contentSize.y);
		}

		 m_LastViewportWindowSize = contentSize;

		 auto wsize = ImGui::GetWindowSize();
		 ImGui::Image((ImTextureID)m_Framebuffer->GetColorAttachment0TextureID(), wsize, ImVec2(0, 1), ImVec2(1, 0));


		ImGuizmo::SetOrthographic(false);
		 ImGuizmo::SetDrawlist();
		
		
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, contentSize.x, contentSize.y);

		

		if (m_CurrentGameObject != entt::null)
		{

			auto& transform = GameInstance::GetCurrentWorld()->GetComponent<Transform>(m_CurrentGameObject);
			auto mat = glm::mat4(transform.GetTransform());
	

			auto& view = m_ViewportCamera->GetViewMatrix();
			auto& proj = m_ViewportCamera->GetProjection();

			if (Input::KeyPressed(KeyCodes::W))
			{
				//translation
				m_GizmoOperation = ImGuizmo::TRANSLATE;
			}
			else if (Input::KeyPressed(KeyCodes::E))
			{
				//rot
				m_GizmoOperation = ImGuizmo::ROTATE;
			}
			else if (Input::KeyPressed(KeyCodes::R))
			{
				//scale
				m_GizmoOperation = ImGuizmo::SCALE;
			}


			ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(proj), m_GizmoOperation, m_GizmoMode,
				glm::value_ptr(mat));


			glm::vec3 pos;
			glm::quat rot;
			glm::vec3 scale;

			
			//ImGui::InputFloat3("Sc", matrixScale, 3);

			if (ImGuizmo::IsUsing())
			{
				Math::DecomposeMatrix(pos, scale, rot, mat);
				glm::vec3 rotEuler = glm::eulerAngles(rot);

				ImGui::Begin("Stuff");
				ImGui::InputFloat3("pos", glm::value_ptr(pos));
				ImGui::InputFloat3("scale", glm::value_ptr(scale));
				ImGui::InputFloat3("rot", glm::value_ptr(rotEuler));
				
				ImGui::End();


				transform.Position = pos;
				transform.Scale = scale;
				transform.Rotation = rotEuler;
			}

		}
		

		

		//ImGuizmo::DrawGrid(glm::value_ptr(view), glm::value_ptr(proj), glm::value_ptr(glm::mat4(1.0f)), 100);

		//spdlog::info(" imguizmo is over {0} is using {1} {2}", ImGuizmo::IsOver(), ImGuizmo::IsUsing(), ImGui::IsMouseClicked(0));

		ImGui::EndChild();

		ImGui::End();

	}

	void ViewportWindow::Update(float ts)
	{

		//spdlog::info("hovered {0} , focused {1}", 1, m_Focused);

		m_ViewportCamera->SetEnableInput(m_Focused);
		if (m_Focused)
		{
			m_ViewportCamera->OnUpdate(ts);
		}
	
		m_Framebuffer->Bind();
		Renderer::Clear({ .2f, .2f, .2f, 1.0f });
		//ViewportRenderer::SetUniformBuffers(m_ViewportCamera->GetViewProjection());

		//Editor rendering here....

		GameInstance::GetCurrentWorld()->Render();

		
	

		m_Framebuffer->UnBind();

	}

}

