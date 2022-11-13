#pragma once
#include <string>

#include "EditorWindow.h"
#include "Engine/Engine/ECS/GameObject.h"
#include "Engine/Engine/Events/Event.h"


namespace Polyboid
{

	class DetailsWindow : public EditorWindow
	{
	private:
		void OnGameObjectSelected(const Event& event);
		void OnGameObjectDeleted(const Event& event);
		GameObject* m_CurrentObject = nullptr;

	public:
		DetailsWindow(const std::string& name);
		void RenderImgui() override;
		void Update(float ts) override;
		~DetailsWindow() override;

	};

}
