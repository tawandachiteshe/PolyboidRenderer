#include "WorldOutlinerWindow.h"
#include <imgui.h>


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

		ImGui::End();

	}

	void WorldOutlinerWindow::Update(float ts)
	{
	}

}

