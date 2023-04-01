#pragma once
#include <any>

#include "Platform/Windows/WindowsWindow.h"

struct ImGuiIO;

namespace Polyboid
{
	class VulkanCommandBuffer;
	class VulkanCommandList;

    class Imgui
    {

    


    public:

        struct ImguiData
        {
            ImGuiIO* io = nullptr;
            GLFWwindow* window;
            VulkanCommandBuffer* m_ImguiCommandBuffer = nullptr;
            VulkanCommandList* m_CommandList = nullptr;
            uint32_t swapChainImage = 0;

        };

        static ImguiData s_Data;

        static  ImguiData& GetData() { return s_Data; }

        static void Init(const std::any& window);
        static void Begin();
        static void End();
        static void ShutDown();
    
    };

}

