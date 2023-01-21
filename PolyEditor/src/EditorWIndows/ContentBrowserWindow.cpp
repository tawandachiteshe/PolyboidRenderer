#include "ContentBrowserWindow.h"


#include <filesystem>
#include <imgui.h>
#include <spdlog/spdlog.h>

#include "Editor/Resource.h"
#include "Engine/Engine/AssetManager.h"


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

		static bool createNewfolder = false;

		if (ImGui::BeginPopupContextWindow())
		{

			if(ImGui::Button("Create Folder"))
			{
				createNewfolder = true;
				
			}

			ImGui::EndPopup();
		}


		if (createNewfolder)
		{
			static char buff[24];

			static bool isFinished;
		
			

			if(ImGui::InputText("Folder Name", buff, 24, 0) && isFinished)
			{
			}

			if (ImGui::IsKeyPressed(ImGuiKey_Enter))
			{
				std::string filename = buff;

				if (!std::filesystem::exists(filename))
				{
					std::filesystem::create_directory(s_CurrentDir / buff);
					createNewfolder = false;
				}

				
			}
		}


		if(ImGui::Button("..."))
		{
			if (s_CurrentDir != s_WorkDir)
			{
				s_CurrentDir = s_CurrentDir.parent_path();
			}
		}

		ImGui::Columns(8, "Folder icons", false);
		

		if (std::filesystem::exists(s_WorkDir))
		{
			for (auto& entry : std::filesystem::directory_iterator(s_CurrentDir))
			{

				if (entry.is_directory())
				{

					const auto pathStr = entry.path().stem().string();
					
					if (ImGui::ImageButton(pathStr.c_str(),(ImTextureID)Resource::GetIcons().at("folder")->GetTextureID(),
						{ 48, 48 }, { 1, 1 }, { 0, 0 }))
					{
						s_CurrentDir = entry.path();

						std::string currentDir = s_CurrentDir.string();

					}


					if (ImGui::BeginDragDropTarget())
					{

						if (const auto payload = ImGui::AcceptDragDropPayload("CONTENT_FILE"))
						{

							std::string filename = static_cast<char*>(payload->Data);
							std::filesystem::copy(s_CurrentDir/filename, s_CurrentDir/pathStr);
							std::filesystem::remove(s_CurrentDir / filename);

						}


						ImGui::EndDragDropTarget();
					}
		

					ImGui::TextWrapped(pathStr.c_str());
					ImGui::NextColumn();

			
				}
				else
				{
					const auto pathStr = entry.path().filename().string();
					static bool clicked = false;

					if(ImGui::ImageButton(pathStr.c_str(), (ImTextureID)Resource::GetIcons().at("file")->GetTextureID(),
						{ 48, 48 }, { 1, 1 }, { 0, 0 }))
					{
						spdlog::info("file {}", pathStr.c_str());

					}

					if (ImGui::BeginPopupContextItem(pathStr.c_str()))
					{

						if(ImGui::Selectable("Proccess Mesh"))
						{
							spdlog::info("file {}", entry.path().string());
							AssetManager::LoadMesh(entry.path().string());
						}
						

						ImGui::EndPopup();
					}

				

					if (ImGui::BeginDragDropSource())
					{
						ImGui::SetDragDropPayload("CONTENT_FILE", pathStr.c_str(), pathStr.size());
						ImGui::Text("%s %d", pathStr.c_str(), pathStr.size());
						ImGui::EndDragDropSource();
					}

					ImGui::TextWrapped(pathStr.c_str());
					ImGui::NextColumn();
				}

				
				
			}
		}


		ImGui::End();

	}

	void ContentBrowserWindow::Update(float ts)
	{
	}
}
