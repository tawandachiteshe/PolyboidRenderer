#include "WorldOutlinerWindow.h"
#include <imgui.h>

#include "Editor/Events/EditorEvents.h"
#include "Engine/Engine/Input.h"
#include "Engine/Engine/ECS/Components.h"
#include "Engine/Engine/Events/EventSystem.h"
#include "Engine/Engine/Gameplay/GameStatics.h"
#include "Engine/Engine/Gameplay/World.h"
#include "Engine/Engine/MeshImporter/MeshImporter.h"
#include "fstream"
#include "Engine/Renderer/Texture3D.h"

namespace Polyboid
{
	WorldOutlinerWindow::WorldOutlinerWindow(const std::string& name)
	{
		m_Name = name;

	}

	WorldOutlinerWindow::~WorldOutlinerWindow()
	{
	}

	void WorldOutlinerWindow::RenderImgui()
	{
		ImGui::Begin(m_Name.c_str());
		auto& registry = GameStatics::GetCurrentWorld()->GetRegistry();

		auto view = registry.view<TagComponent, IDComponent>();

		// static bool open = true;
		// ImGui::ShowDemoWindow(&open);


		if (ImGui::TreeNodeEx("Game Objects", ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen))
		{
			for (auto entity : view.each())
			{
				auto [entityHandle, tag, id] = entity;

				static std::string id_string = "##";

				//spdlog::info("cuurent id {} , m_cuuertnt GameObejcy {}", (uint64_t)id.id, m_CurrentGameObject);

				ImGui::PushID(std::to_string(id.id).c_str());
				if (ImGui::Selectable(tag.name.c_str(), m_CurrentGameObject == id.id))
				{
					GameObjectOutlineClick event(GameStatics::GetCurrentWorld()->FindGameObjectByID(id.id));
					EventSystem::GetDispatcher()->Dispatch(event);
					m_CurrentGameObject = id.id;
				}
				ImGui::PopID();

				if (ImGui::BeginPopupContextItem())
				{
					if(ImGui::Button("Delete"))
					{
						auto* gameObject = GameStatics::GetCurrentWorld()->FindGameObjectByID(m_CurrentGameObject);
						if (gameObject != nullptr)
						{
							GameObjectOutlineDeleted event;
							EventSystem::GetDispatcher()->Dispatch(event);
							GameStatics::GetCurrentWorld()->DestroyGameObject(gameObject);
							m_CurrentGameObject = 0;
						}
					}

					ImGui::EndPopup();
				}

				if (Input::KeyPressed(KeyCodes::DELETE_KEY))
				{
					auto* gameObject = GameStatics::GetCurrentWorld()->FindGameObjectByID(m_CurrentGameObject);
					if (gameObject != nullptr)
					{
						GameObjectOutlineDeleted event;
						EventSystem::GetDispatcher()->Dispatch(event);
						GameStatics::GetCurrentWorld()->DestroyGameObject(gameObject);
						m_CurrentGameObject = 0;
					}
				}

				

			}

			ImGui::TreePop();
		}


		ImGui::End();
	}

	void WorldOutlinerWindow::Update(float ts)
	{
	}
}
