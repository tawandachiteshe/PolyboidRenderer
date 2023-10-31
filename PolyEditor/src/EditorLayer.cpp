#include "EditorLayer.h"
#include <imgui.h>

#include "Engine/Engine/Application.h"
#include "Engine/Engine/Events/EventDispatcher.h"
#include <map>

//temp solution temp
#include <Windows.h>
#include <commdlg.h>


#include "Engine/Renderer/RenderCommand.h"
#include "Windows/OutlineWindow.h"

namespace Polyboid
{
	static std::filesystem::path s_AssetsPath = "Assets";

	static std::string OpenWindowsDialog()
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = {0};
		CHAR currentDir[256] = {0};
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		//ofn.hwndOwner = glfwGetWin32Window((Application::Get().GetWindow()->GetNativeWindow()));
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		if (GetCurrentDirectoryA(256, currentDir))
			ofn.lpstrInitialDir = currentDir;
		ofn.lpstrFilter = "(*.pmap)\0*.pmap\0";
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA(&ofn) == TRUE)
			return ofn.lpstrFile;

		return std::string();
	}


	std::string SaveWindowsDialog()
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = {0};
		CHAR currentDir[256] = {0};
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		// ofn.hwndOwner = glfwGetWin32Window((Application::Get()->GetWindow()->GetNativeWindow()));
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		if (GetCurrentDirectoryA(256, currentDir))
			ofn.lpstrInitialDir = currentDir;
		ofn.lpstrFilter = "(*.pmap)\0*.pmap\0";
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

		// Sets the default extension by extracting it from the filter
		ofn.lpstrDefExt = strchr(ofn.lpstrFilter, '\0') + 1;

		if (GetSaveFileNameA(&ofn) == TRUE)
			return ofn.lpstrFile;

		return std::string();
	}


	void EditorLayer::EditorDockSpaceUI(bool* p_open)
	{
		// If you strip some features of, this demo is pretty much equivalent to calling DockSpaceOverViewport()!
		// In most cases you should be able to just call DockSpaceOverViewport() and ignore all the code below!
		// In this specific demo, we are not using DockSpaceOverViewport() because:
		// - we allow the host window to be floating/moveable instead of filling the viewport (when opt_fullscreen == false)
		// - we allow the host window to have padding (when opt_padding == true)
		// - we have a local menu bar in the host window (vs. you could use BeginMainMenuBar() + DockSpaceOverViewport() in your code!)
		// TL;DR; this demo is more complicated than what you would normally use.
		// If we removed all the options we are showcasing, this demo would become:
		//     void ShowExampleAppDockSpace()
		//     {
		//         ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		//     }

		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

		ImGuiViewport* viewport = ImGui::GetMainViewport();

		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", p_open, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		else
		{
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.

				if (ImGui::MenuItem("New...", "Ctrl + Shift + N"))
				{
					auto pathDialog = SaveWindowsDialog();

					if (!pathDialog.empty())
					{
						std::filesystem::path path = pathDialog;
						auto stem = path.stem();
						std::string worldName = stem.string();
					}
				}

				if (ImGui::MenuItem("Save as..."))
				{
					auto pathDialog = SaveWindowsDialog();

					if (!pathDialog.empty())
					{
						//WorldSerializer::Serialize(pathDialog);
					}
				}

				if (ImGui::MenuItem("Save World", "Ctrl + S"))
				{
					//temp solution
					// WorldSerializer::Serialize(Editor::GetCurrentWorldMapPath());
				}

				if (ImGui::MenuItem("Load World", "Ctrl + O"))
				{
					auto str = OpenWindowsDialog();

					if (!str.empty())
					{
						//WorldSerializer::Deserialize(str);
					}
				}

				// if (ImGui::MenuItem("Reload C#", "Ctrl + R"))
				// {
				//
				//     for (auto gameObject : GameStatics::GetCurrentWorld()->GetGameObjects())
				//     {
				//         gameObject->ClearScripts();
				//     }
				//
				//     ScriptingEngine::ReloadAssembly();
				//
				//     for (auto gameObject : GameStatics::GetCurrentWorld()->GetGameObjects())
				//     {
				//         gameObject->ReAttachScripts();
				//     }
				// }

				if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
					*p_open = false;
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}


		ImGui::End();
	}


	EditorLayer::EditorLayer(const std::string& name)
	{
		//float fov, float aspectRatio, float nearClip, float farClip

		m_Name = name;

		//TODO be careful
		//offscreenBuffers->Recreate();


	}

	void EditorLayer::OnAttach()
	{

		m_CurrentWorld = World::Create("Untitled");

		m_OutlineWindow = Ref(new OutlineWindow(m_CurrentWorld));


	}

	void EditorLayer::OnUpdate(float dt)
	{



	}


	void EditorLayer::OnImguiRender()
	{
		bool initDockSpace = true;
		EditorDockSpaceUI(&initDockSpace);
		m_OutlineWindow->RenderUi();


		//ViewPort
		ImGui::Begin("Viewport");

		ImGui::End();



	
	}

	void EditorLayer::OnRender()
	{
		
	}

	void EditorLayer::OnEvent(Event& event)
	{
		
	}
}
