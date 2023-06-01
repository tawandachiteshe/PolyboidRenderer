#pragma once
#include <any>

#include "imgui.h"
#include "Platform/Windows/WindowsWindow.h"

struct ImGuiIO;

namespace Polyboid
{
	class Texture;
	class CommandList;
	class VulkanRenderPass;
	class VulkanCommandBuffer;
	class VulkanCommandList;



    class Imgui
    {

        


    public:

        static ImTextureID GetVulkanTextureID(const Ref<Texture>& texture);


        struct ImguiData
        {
            ImGuiIO* io = nullptr;
            GLFWwindow* window = nullptr;
        	Ref<VulkanCommandList> m_CommandList = nullptr;
            Ref<VulkanRenderPass> m_RenderPass = nullptr;
            uint32_t swapChainImage = 0;
            std::any m_CommandPool;

        };

        static ImguiData s_Data;

        static  ImguiData& GetData() { return s_Data; }

        static void Init(const std::any& window);
        static void Begin(const Ref<CommandList>& cmdList);
        static void End();
        static void ShutDown();
    
    };

}

