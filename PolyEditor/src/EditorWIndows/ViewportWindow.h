#pragma once

#include "EditorWindow.h"
#include <string>
#include <entt/entt.hpp>

#include "EditorLayer.h"
#include "imgui.h"
#include "ImGuizmo.h"
#include "Engine/Engine/Base.h"
#include "Engine/Renderer/Framebuffer.h"
#include "Engine/Renderer/UniformBuffer.h"
#include "glm/vec2.hpp"

namespace Polyboid
{
	class Material;
	class MouseScrollEvent;
	class GameObject;
	class EditorCamera;


	class ViewportWindow : public EditorWindow
	{
	
	public:
		ViewportWindow(const std::string& name);
		~ViewportWindow() override = default;
		Ref<EditorCamera> m_ViewportCamera;

		ImVec2 m_LastViewportWindowSize = { 0.0f, 0.0f };
		ImVec2 m_WindowOffset = { 0, 0 };

		std::vector<ImTextureID> m_FramebufferTextures;

		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;

		Ref<VertexBuffer> m_VertexBuffer2;
		Ref<IndexBuffer> m_IndexBuffer2;

		Vertex m_Vertices[4];
		Vertex m_VerticesBuffer[4];
		Ref<Material> m_RedMaterial;
		Ref<Material> m_BlueMaterial;

		
		float m_Rotation;

	private:
		GameObject* m_CurrentGameObject = nullptr;
		bool m_Playmode = false;
		bool m_Focused = false;
		bool m_OnResize = false;
		glm::vec2 m_LastViewportSize = {1, 1};
		ImGuizmo::OPERATION m_GizmoOperation = ImGuizmo::TRANSLATE;
		ImGuizmo::MODE m_GizmoMode = ImGuizmo::LOCAL;

		void OnGameObjectSelected(const Event& event);
		void OnGameObjectDeleted(const Event& event);

		void ResizeViewportFrameBuffers(const glm::uvec2& size);

		ImTextureID GetImGuiCompositeTexture() const;

		// Inherited via EditorWindow
		virtual void RenderImgui() override;

		virtual void Update(float ts) override;

		void OnEvent(Event& event) override;


	public:
		void OnRender() override;
	};

}
