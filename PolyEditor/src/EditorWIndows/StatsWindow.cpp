#include "StatsWindow.h"
#include "imgui.h"

namespace Polyboid
{
	StatsWindow::StatsWindow()
	{
		m_Name = "Settings";
	}

	void StatsWindow::RenderImgui()
	{
		ImGui::Begin(m_Name.c_str());

		auto frameRate = ImGui::GetIO().Framerate;

		ImGui::Text("Frame rate %f", (1.0f / frameRate));
		

		ImGui::End();
	}

	void StatsWindow::Update(float ts)
	{
	}

	StatsWindow::~StatsWindow()
	{
	}
}
