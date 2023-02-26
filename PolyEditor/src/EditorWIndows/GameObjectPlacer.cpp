#include "GameObjectPlacer.h"
#include <imgui.h>
#include <Editor/Resource.h>
#include <spdlog/spdlog.h>


namespace Polyboid 
{
	GameObjectPlacer::GameObjectPlacer(const std::string& name)
	{
		m_Name = name;
		m_GameObjects.push_back(GameObjectsType::EMPTY);
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
