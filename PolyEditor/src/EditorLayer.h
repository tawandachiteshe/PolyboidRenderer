#pragma once

#include "Engine/Engine/Layer.h"
#include "EditorWIndows/EditorWindow.h"
#include <vector>

#include "Editor/EditorCamera.h"
#include "Engine/Engine/Base.h"


namespace Polyboid 
{

	class EditorLayer: public Layer
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
	};

	
}

