#include "GameObjectPlacer.h"
#include <imgui.h>
#include <Editor/Resource.h>
#include <spdlog/spdlog.h>

#include "Engine/Engine/Gameplay/GameInstance.h"
#include "Engine/Engine/Gameplay/World.h"

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

		// ImGui::Columns(12, "GameObjectsPlacer", false);

		for (auto gameObjectType : m_GameObjects)
		{
			switch (gameObjectType)
			{
			case GameObjectsType::EMPTY:
				if(ImGui::ImageButton("EmptyButton", (ImTextureID)Resource::GetIcons().at("game_object")->GetTextureID(), { 48, 48 }, { 1, 1 }, { 0, 0 }))
				{
					
				}

				if (ImGui::BeginPopupContextItem())
				{
					static char name[128];
					if (ImGui::InputText("GameObject", name, 128, ImGuiInputTextFlags_EnterReturnsTrue))
					{
						spdlog::info("buffer data{}", name);
						GameInstance::GetCurrentWorld()->CreateGameObject(name);
					}
					
					ImGui::EndPopup();
				}

				ImGui::SetCursorPosX(20);
				ImGui::TextWrapped("Empty");
				break;
			default:
				break;
			}
			//
			// ImGui::TextWrapped("Empty");
			// ImGui::NextColumn();
		}

		ImGui::End();
	}


	void GameObjectPlacer::Update(float ts)
	{
	}
}
