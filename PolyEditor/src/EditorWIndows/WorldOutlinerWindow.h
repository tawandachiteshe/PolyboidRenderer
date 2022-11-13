#pragma once

#include "EditorWindow.h"
#include <string>
#include <entt/entt.hpp>

#include "Engine/Engine/UUID.h"


namespace Polyboid 
{
	class WorldOutlinerWindow : public EditorWindow
	{
	private:
		UUID m_CurrentGameObject = 0;
	public:
		WorldOutlinerWindow(const std::string& name);
		virtual ~WorldOutlinerWindow();

		// Inherited via EditorWindow
		virtual void RenderImgui() override;

		virtual void Update(float ts) override;

	};

}
