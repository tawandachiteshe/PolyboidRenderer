#include "ContentBrowserWindow.h"


#include <filesystem>
#include <future>
#include <imgui.h>
#include <spdlog/spdlog.h>

#include "Editor/Resource.h"
#include "Engine/Engine/Application.h"


namespace Polyboid
{
	static std::filesystem::path s_WorkDir = "Assets";
	static std::filesystem::path s_CurrentDir = s_WorkDir;

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
