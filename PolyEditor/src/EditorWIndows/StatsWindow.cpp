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

		double renderTimert = Application::Get()->GetStats().m_RenderTime;
		double gameTimert = Application::Get()->GetStats().m_GameTime;

		static double renderTime = 0.0;
		static double gameTime = 0.0;

		static double renderTimeGUI = 0.0;
		static double gameTimeGUI = 0.0;

		if (renderTime >= 1.0)
		{
			renderTimeGUI = renderTimert;
			renderTime = 0;
		}

		if (gameTime >= 1.0)
		{
			gameTimeGUI = gameTimert;
			gameTime = 0;
		}

		renderTime += renderTimert;
		gameTime += gameTimert;

		ImGui::Text("Render Frame time %f ms", renderTimeGUI);
		ImGui::Text("Render Frames per second %f", 1.0 / renderTimeGUI);

		ImGui::Text("Game Frame time %f ms", gameTimeGUI);
		ImGui::Text("Game Frames per second %f", 1.0 / gameTimeGUI);

		ImGui::End();
	}

	void StatsWindow::Update(float ts)
	{
	}

	StatsWindow::~StatsWindow()
	{
	}
}
