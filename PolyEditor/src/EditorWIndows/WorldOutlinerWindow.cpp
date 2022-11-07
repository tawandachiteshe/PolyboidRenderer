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

		auto view = registry.view<Tag>();

		for (auto entity :  view.each())
		{
			auto& [entityHandle, tag] = entity;
		;
			
			if (ImGui::TreeNodeEx(tag.name.c_str(), ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen))
			{
				
				if (ImGui::IsItemClicked())
				{
					GameObjectOutlineClick event(static_cast<uint32_t>(entityHandle));
					EventSystem::GetDispatcher()->Dispatch(event);
					spdlog::info("{}", static_cast<uint32_t>(entityHandle));
				}
		

				ImGui::TreePop();
			}
			
		}

		ImGui::End();

	}

	void WorldOutlinerWindow::Update(float ts)
	{
	}

}

