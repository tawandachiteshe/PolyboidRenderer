#pragma once

#include "Engine/Engine/Layer.h"
#include "EditorWIndows/EditorWindow.h"
#include <vector>


namespace Polyboid 
{
	class EditorLayer: public Layer
	{

	private:
		std::vector<EditorWindow*> m_Windows;
		void EditorDockSpaceUI(bool* p_open);
		void AddWindow(EditorWindow* window);

	public:

		EditorLayer(const std::string& name) { m_Name = name; };
		void OnAttach() override;
		void OnUpdate(float dt) override;
		void OnImguiRender() override;
		
	};

}

