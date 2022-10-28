#include "ViewportWindow.h"
#include <imgui.h>


namespace Polyboid

{
	ViewportWindow::ViewportWindow(const std::string& name)
	{
		m_Name = name;
	}

	ViewportWindow::~ViewportWindow()
	{
	}

	void ViewportWindow::RenderImgui()
	{

		ImGui::Begin(m_Name.c_str());

		ImGui::End();

	}

	void ViewportWindow::Update(float ts)
	{



	}

}

