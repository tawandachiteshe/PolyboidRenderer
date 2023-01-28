#include "StatsWindow.h"

#include "imgui.h"
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

		ImGui::Text("Render Frame time %f ms", GameStatics::GetCurrentWorld()->GetRenderTime());
		ImGui::Text("Render Frames per second %.0f ms", 1000.0 / GameStatics::GetCurrentWorld()->GetRenderTime());

		ImGui::End();
	}

	void StatsWindow::Update(float ts)
	{
	}

	StatsWindow::~StatsWindow()
	{
	}
}
