#pragma once
#include <any>

#include "imgui.h"
#include "Platform/Windows/WindowsWindow.h"

struct ImGuiIO;

namespace Polyboid
{
	class CommandBuffer;
	class Texture2D;
	class CommandBufferSet;
	class VulkanRenderPass;
	class VulkanCommandBuffer;
	class VulkanCommandBufferSet;



    class Imgui
    {

        


    public:

        static ImTextureID CreateVulkanTextureID(const Ref<Texture2D>& texture);
        static void FreeVulkanTextureID(const ImTextureID& texture);


        struct ImguiData
        {
            ImGuiIO* io = nullptr;
            GLFWwindow* window = nullptr;
        	Ref<VulkanCommandBufferSet> m_CommandList = nullptr;
            Ref<VulkanRenderPass> m_RenderPass = nullptr;
            std::any m_DescPool;
            uint32_t swapChainImage = 0;
            std::any m_CommandPool;

        };

        static ImguiData s_Data;

        static  ImguiData& GetData() { return s_Data; }

        static void Init(const std::any& window);
        static void InitVulkanRenderer();
        static void DestroyVulkanRenderer();
        static void RecreateVulkanRenderer();
        static void Begin();
        static void End();
        static void SubmitToCommandBuffer(const Ref<CommandBuffer>& cmdBuffer);
        static void ShutDown();
    
    };

}

