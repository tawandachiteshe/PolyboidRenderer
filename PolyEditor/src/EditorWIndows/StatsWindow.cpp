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

		ImGui::Text("Frame rate %.1f", ImGui::GetIO().Framerate);
		

		ImGui::End();
	}

	void StatsWindow::Update(float ts)
	{
	}

	StatsWindow::~StatsWindow()
	{
	}
}
