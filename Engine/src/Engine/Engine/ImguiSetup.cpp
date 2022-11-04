#include "ImguiSetup.h"

//#define  IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#include "Application.h"
#include "imgui.h"
#include "ImGuizmo.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"


namespace Polyboid
{
	struct ImguiData
	{
        ImGuiIO* io = nullptr;
	};


    static ImguiData s_Data;

    void Imgui::Init()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        s_Data.io = &io;

        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
            style.Colors[ImGuiCol_FrameBgActive] = { 0.2f, 0.2f, 0.2f, 1.0f };
            style.Colors[ImGuiCol_FrameBgHovered] = { 0.2f, 0.2f, 0.2f, 1.0f };
            style.Colors[ImGuiCol_Border] = { 0.2f, 0.2f, 0.2f, 1.0f };
            style.Colors[ImGuiCol_Header] = { 0.2f, 0.2f, 0.2f, 1.0f };
            style.Colors[ImGuiCol_HeaderActive] = { 0.2f, 0.2f, 0.2f, 1.0f };
            style.Colors[ImGuiCol_HeaderHovered] = { 0.2f, 0.2f, 0.2f, 1.0f };
            style.Colors[ImGuiCol_TitleBg] = { 0.2f, 0.2f, 0.2f, 1.0f };
            style.Colors[ImGuiCol_TitleBgActive] = { 0.2f, 0.2f, 0.2f, 1.0f };
            style.Colors[ImGuiCol_HeaderHovered] = { 0.2f, 0.2f, 0.2f, 1.0f };

            style.Colors[ImGuiCol_Tab] = { 0.2f, 0.2f, 0.2f, 1.0f };
            style.Colors[ImGuiCol_TabHovered] = { 0.2f, 0.2f, 0.2f, 1.0f };
            style.Colors[ImGuiCol_TabActive] = { 0.4f, 0.4f, 0.4f, 1.0f };
            style.Colors[ImGuiCol_TabUnfocusedActive] = { 0.2f, 0.2f, 0.2f, 1.0f };
            style.Colors[ImGuiCol_TabUnfocused] = { 0.2f, 0.2f, 0.2f, 1.0f };

            style.Colors[ImGuiCol_Button] = { 0.1f, 0.1f, 0.1f, 1.0f };
            style.Colors[ImGuiCol_ButtonActive] = { 0.2f, 0.2f, 0.2f, 1.0f };
            style.Colors[ImGuiCol_ButtonHovered] = { 0.2f, 0.2f, 0.2f, 1.0f };

            style.TabRounding = 0.0f;
            style.FrameRounding = 0.0f;
            style.PopupRounding = 0.0f;
            style.ChildRounding = 0.0f;
        }

        ImGui_ImplGlfw_InitForOpenGL(Application::Get()->GetWindow()->GetNativeWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 150");

      
      
    }

    void Imgui::Begin()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();

    }

    void Imgui::End()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if ((*s_Data.io).ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void Imgui::ShutDown()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}
