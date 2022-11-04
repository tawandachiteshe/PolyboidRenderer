#include "WorldOutlinerWindow.h"
#include <imgui.h>

#include "Engine/Engine/ECS/Components.h"
#include "Engine/Engine/Gameplay/GameInstance.h"
#include "Engine/Engine/Gameplay/World.h"


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

		for (auto entity : view.each())
		{
			auto [_, tag] = entity;

			auto size = ImGui::GetWindowContentRegionWidth();

			ImGui::Button(tag.name.c_str(), { size, 25  });
			
		}

		ImGui::End();

	}

	void WorldOutlinerWindow::Update(float ts)
	{
	}

}

