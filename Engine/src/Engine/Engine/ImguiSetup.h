#pragma once
#include <any>

#include "Platform/Windows/WindowsWindow.h"

struct ImGuiIO;

namespace Polyboid
{
	class CommandList;
	class VulkanDescriptorPool;
	class VulkanRenderPass;
	class VulkanCommandBuffer;
	class VulkanCommandList;

    class Imgui
    {

    


    public:

        struct ImguiData
        {
            ImGuiIO* io = nullptr;
            GLFWwindow* window;
        	Ref<VulkanCommandList> m_CommandList = nullptr;
            Ref<VulkanRenderPass> m_RenderPass = nullptr;
            uint32_t swapChainImage = 0;
            Ref<VulkanDescriptorPool> cmdPool;

        };

        static ImguiData s_Data;

        static  ImguiData& GetData() { return s_Data; }

        static void Init(const std::any& window);
        static void Begin(const Ref<CommandList>& cmdList);
        static void End();
        static void ShutDown();
    
    };

}

