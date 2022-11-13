#include "WorldOutlinerWindow.h"
#include <imgui.h>

#include "Editor/Events/EditorEvents.h"
#include "Engine/Engine/ECS/Components.h"
#include "Engine/Engine/Events/EventSystem.h"
#include "Engine/Engine/Gameplay/GameInstance.h"
#include "Engine/Engine/Gameplay/World.h"
#include "spdlog/spdlog.h"


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
		auto& registry = GameInstance::GetCurrentWorld()->GetRegistry();

		auto view = registry.view<TagComponent, IDComponent>();

		static bool open = true;
		ImGui::ShowDemoWindow(&open);


		if (ImGui::TreeNodeEx("Game Objects", ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen))
		{
			for (auto entity : view.each())
			{
				auto& [entityHandle, tag, id] = entity;

				static std::string id_string = "##";

				ImGui::PushID(std::to_string(id.id).c_str());
				if (ImGui::Selectable(tag.name.c_str(), id.id == m_CurrentGameObject))
				{

					GameObjectOutlineClick event(GameInstance::GetCurrentWorld()->FindGameObjectByID(id.id));
					EventSystem::GetDispatcher()->Dispatch(event);
					m_CurrentGameObject = id.id;
				}

				if (ImGui::BeginPopupContextItem())
				{
					if(ImGui::Button("Delete"))
					{
						auto* gameObject = GameInstance::GetCurrentWorld()->FindGameObjectByID(m_CurrentGameObject);
						if (gameObject != nullptr)
						{
							GameObjectOutlineDeleted event;
							EventSystem::GetDispatcher()->Dispatch(event);
							GameInstance::GetCurrentWorld()->DestroyGameObject(gameObject);
							m_CurrentGameObject = 0;
						}
					}

					ImGui::EndPopup();
				}

				ImGui::PopID();

			}

			ImGui::TreePop();
		}


		ImGui::End();
	}

	void WorldOutlinerWindow::Update(float ts)
	{
	}
}
