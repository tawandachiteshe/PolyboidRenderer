#pragma once

#include "Engine/Engine/Base.h"
#include "Engine/Engine/Layer.h"

namespace Polyboid
{
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
		bool m_PlayMode = false;
		Ref<OutlineWindow> m_OutlineWindow;
		Ref<World> m_CurrentWorld;
	};
}
