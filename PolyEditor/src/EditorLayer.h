#pragma once

#include "Engine/Engine/Layer.h"
#include "EditorWIndows/EditorWindow.h"
#include <vector>

#include "imgui.h"
#include "Editor/EditorCamera.h"
#include "Engine/Engine/Base.h"
#include "Engine/Renderer/BufferSet.h"
#include "Engine/Renderer/CommandBufferSet.h"


namespace Polyboid
{
	struct Vertex
	{
		glm::vec3 pos;
		glm::vec4 norm;
		glm::vec2 uv;
	};




	class EditorLayer : public Layer
	{
	private:
		std::vector<EditorWindow*> m_Windows;
		void EditorDockSpaceUI(bool* p_open);
		void AddWindow(EditorWindow* window);
		bool m_PlayMode = false;
		void OnEditorEnterPlayMode(const Event& event);
		void OnEditorExitPlayMode(const Event& event);


	public:


		EditorLayer(const std::string& name);
		void OnAttach() override;
		void OnUpdate(float dt) override;
		void OnImguiRender() override;
		void OnRender() override;

		void OnEvent(Event& event) override;

		~EditorLayer() override = default;
	};
}
