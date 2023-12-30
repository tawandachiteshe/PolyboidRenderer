#pragma once

#include <vector>
#include <glm/vec2.hpp>

#include "imgui.h"
#include "ImGuizmo.h"
#include "Engine/Engine/Base.h"
#include "Engine/Engine/Layer.h"

namespace Polyboid
{
	class EditorCamera;
	class CommandBufferSet;
	class World;
	class OutlineWindow;

	class EditorLayer : public Layer
	{

	public:

		EditorLayer(const std::string& name);
		void OnAttach() override;
		void OnUpdate(float dt) override;
		void OnImguiRender() override;
		void OnRender() override;

		void OnEvent(Event& event) override;

		~EditorLayer() override = default;

	private:
		void EditorDockSpaceUI(bool* p_open);
		void BuildRenderImage();
		ImTextureID GetViewportImageRender() const;

	private:

		glm::vec2 m_ViewportBounds[2]{};
		bool m_PlayMode = false;
		Ref<OutlineWindow> m_OutlineWindow;
		Ref<World> m_CurrentWorld;

		Ref<CommandBufferSet> m_EditorCommandBuffer;
		Ref<EditorCamera> m_EditorCamera;
		std::vector<ImTextureID> m_RenderViewportImage;
		glm::uvec2 m_LastViewportSize{1};


		ImGuizmo::OPERATION m_GizmoOp = ImGuizmo::TRANSLATE;
		ImGuizmo::MODE m_GizmoMode = ImGuizmo::MODE::WORLD;

		bool m_FirstResize = true;

	};
}
