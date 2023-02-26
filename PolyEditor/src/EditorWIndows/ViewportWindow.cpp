#include "ViewportWindow.h"
#include <imgui.h>
#include <spdlog/spdlog.h>

#include "Editor/EditorCamera.h"
#include "Engine/Engine/Events/EventDispatcher.h"
#include "Engine/Engine/Engine.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Renderer/RenderTarget.h"
#include "glm/glm.hpp"

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

		ImGui::Image((ImTextureID)colorTexture->GetHandle(), windowSize);

		ImGui::End();

		ImGui::PopStyleVar();

	}

	void ViewportWindow::Update(float ts)
	{

	}

	void ViewportWindow::OnRender()
	{
		Renderer2D::BeginDraw(m_ViewportCamera);

		//Renderer2D::DrawQuad(glm::mat4(1.0f));
		Renderer2D::DrawCircle(glm::mat4(1.0f));

		Renderer2D::EndDraw();
	}
}

