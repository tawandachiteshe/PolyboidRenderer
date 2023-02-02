#include "GameObjectPlacer.h"
#include <imgui.h>
#include <Editor/Resource.h>
#include <spdlog/spdlog.h>

#include "Engine/Engine/Gameplay/GameStatics.h"
#include "Engine/Engine/Gameplay/World.h"
#include "Engine/Engine/Scripting/ScriptingEngine.h"

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

		ImGui::Columns(4, "GameObjectsPlacer", false);

		for (const auto& name : ScriptingEngine::GetClasses())
		{
			if (ImGui::ImageButton(name.c_str(), (ImTextureID)Resource::GetIcons()["game_object"]->GetTextureID(), { 48, 48 }, { 1, 1 }, { 0, 0 }))
			{
				auto handle = GameStatics::GetCurrentWorld()->CreateGameObject(name);
				handle->AttachScript(name);
			}
			ImGui::TextWrapped(name.c_str());
			ImGui::NextColumn();
		}


		ImGui::Columns(4, "GameObjectsPlacer", false);
		//prebuild objects
		for (const auto gameObjectType : m_GameObjects)
		{
			switch (gameObjectType)
			{
			case GameObjectsType::EMPTY:
				if(ImGui::ImageButton("EmptyButton", (ImTextureID)Resource::GetIcons()["game_object"]->GetTextureID(), {48, 48}, {1, 1}, {0, 0}))
				{
					
				}

				if (ImGui::BeginPopupContextItem())
				{
					static char name[128];
					if (ImGui::InputText("GameObject", name, 128, ImGuiInputTextFlags_EnterReturnsTrue))
					{
						GameStatics::GetCurrentWorld()->CreateGameObject(name);
					}
					
					ImGui::EndPopup();
				}

				ImGui::TextWrapped("Empty");
				ImGui::NextColumn();
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
