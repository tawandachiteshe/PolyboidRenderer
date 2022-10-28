#include "GameObjectPlacer.h"
#include <imgui.h>

namespace Polyboid 
{
	GameObjectPlacer::GameObjectPlacer(const std::string& name)
	{
		m_Name = name;
	}


	GameObjectPlacer::~GameObjectPlacer()
	{
	}


	void GameObjectPlacer::RenderImgui()
	{

		ImGui::Begin(m_Name.c_str());

		ImGui::End();

	}


	void GameObjectPlacer::Update(float ts)
	{
	}
}