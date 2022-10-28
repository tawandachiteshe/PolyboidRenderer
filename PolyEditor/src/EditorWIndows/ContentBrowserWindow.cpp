#include "ContentBrowserWindow.h"
#include <imgui.h>

namespace Polyboid 
{
	ContentBrowserWindow::ContentBrowserWindow(const std::string& name)
	{
		m_Name = name;
	}

	ContentBrowserWindow::~ContentBrowserWindow()
	{
	}

	void ContentBrowserWindow::RenderImgui()
	{

		ImGui::Begin(m_Name.c_str());

		ImGui::End();

	}

	void ContentBrowserWindow::Update(float ts)
	{
	}
}