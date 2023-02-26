#pragma once

#include "EditorWindow.h"
#include <string>
#include <entt/entt.hpp>

#include "imgui.h"
#include "ImGuizmo.h"
#include "Engine/Engine/Base.h"
#include "Engine/Renderer/Framebuffer.h"
#include "Engine/Renderer/UniformBuffer.h"
#include "glm/vec2.hpp"

namespace Polyboid
{
	class GameObject;
	class EditorCamera;


	class ViewportWindow : public EditorWindow
	{
	
	public:
		ViewportWindow(const std::string& name);
		virtual ~ViewportWindow();
		Ref<EditorCamera> m_ViewportCamera;

		ImVec2 m_LastViewportWindowSize = { 0.0f, 0.0f };
		ImVec2 m_WindowOffset = { 0, 0 };


	private:
		GameObject* m_CurrentGameObject = nullptr;
		bool m_Playmode = false;
		bool m_Focused = false;
		ImGuizmo::OPERATION m_GizmoOperation = ImGuizmo::TRANSLATE;
		ImGuizmo::MODE m_GizmoMode = ImGuizmo::LOCAL;

		void OnGameObjectSelected(const Event& event);
		void OnGameObjectDeleted(const Event& event);

		// Inherited via EditorWindow
		virtual void RenderImgui() override;

		virtual void Update(float ts) override;

	public:
		void OnRender() override;
	};

}
