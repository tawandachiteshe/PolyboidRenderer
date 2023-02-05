#include "StatsWindow.h"

#include "imgui.h"
#include "Engine/Engine/Application.h"
#include "Engine/Engine/Gameplay/GameStatics.h"
#include "Engine/Engine/Gameplay/World.h"

namespace Polyboid
{
	StatsWindow::StatsWindow()
	{
		m_Name = "Settings";
	}

	void StatsWindow::RenderImgui()
	{
		ImGui::Begin(m_Name.c_str());

		auto& stats = Application::Get()->GetStats();


		ImGui::Text("Render Frame time %f ms", stats.RenderTimeMs);
		ImGui::Text("Render Frames per second %f", 1000.0 / stats.RenderTimeMs);

		ImGui::Text("Game Frame time %f ms", stats.GameTimeMs);
		ImGui::Text("Game Frames per second %f", 1000.0 / stats.GameTimeMs);

		ImGui::End();
	}

	void StatsWindow::Update(float ts)
	{
	}

	StatsWindow::~StatsWindow()
	{
	}
}
