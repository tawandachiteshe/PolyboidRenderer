#include "boidpch.h"

#include "ImguiSetup.h"

//#define  IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#include <spdlog/spdlog.h>

#include "Application.h"
#include "imgui.h"
#include "ImGuizmo.h"
#include "imgui_impl_glfw.h"
#include "Engine/Renderer/RenderAPI.h"
#include "GLFW/glfw3.h"
#include "imgui_impl_vulkan.h"
#include "Engine/Renderer/Renderer.h"
#include "Platform/Vulkan/VkRenderAPI.h"
#include "Platform/Vulkan/VulkanCommandBuffer.h"
#include "Platform/Vulkan/VulkanRenderPass.h"
#include "Platform/Vulkan/Utils/VkInstance.h"
#include "Platform/Vulkan/Utils/VulkanDevice.h"
#include "Platform/Vulkan/Utils/VulkanPhysicalDevice.h"
#include "Platform/Vulkan/Utils/VulkanSurfaceKHR.h"
#include "Platform/Vulkan/VkSwapChain.h"
#include "Platform/Vulkan/VulkanCommandBufferSet.h"
#include "vulkan/vulkan_raii.hpp"


namespace Polyboid
{


	Imgui::ImguiData Imgui::s_Data;
    

    static void check_vk_result(VkResult err)
    {

        if (err == 0)
            return;
        fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
        if (err < 0)
            __debugbreak();
    }


    ImTextureID Imgui::GetVulkanTextureID(const Ref<Texture>& texture)
    {
        auto vulkanSampler = std::any_cast<vk::Sampler>(texture->GetSamplerHandle());
        auto vulkanView = std::any_cast<vk::ImageView>(texture->GetViewHandle());

    	auto ImguiDS = ImGui_ImplVulkan_AddTexture(vulkanSampler, vulkanView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        return ImguiDS;
    }

    //TODO make this api agnostic
    void Imgui::Init(const std::any& window)
    {

    	IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        
        io.Fonts->AddFontFromFileTTF("Resources/Fonts/roboto_mono.ttf", 16);

        s_Data.window = std::any_cast<GLFWwindow*>(window);
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

     
        auto type = RenderAPI::Get()->GetRenderAPIType();

        //g_MainWindowData.
      

        if (type == RenderAPIType::Vulkan)
        {
            auto renderAPI = dynamic_cast<VkRenderAPI*>(RenderAPI::Get());
  

           


            // Select Surface Format
            ImGui_ImplGlfw_InitForVulkan((s_Data.window), true);

            InitVulkanRenderer();

        }



    }

    void Imgui::InitVulkanRenderer()
    {

       

        auto renderAPI = dynamic_cast<VkRenderAPI*>(RenderAPI::Get());
        ImGui_ImplVulkan_InitInfo init_info = {};

        VkDescriptorPoolSize pool_sizes[] =
        {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
        };

        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 1000;
        pool_info.poolSizeCount = std::size(pool_sizes);
        pool_info.pPoolSizes = pool_sizes;
        s_Data.m_RenderPass = Renderer::GetSwapChain()->GetRenderPass().As<VulkanRenderPass>();

        VkDescriptorPool imguiPool;
        (vkCreateDescriptorPool(renderAPI->GetDevice()->GetVulkanDevice(), &pool_info, nullptr, &imguiPool));
        vk::DescriptorPool pool = imguiPool;
        s_Data.m_CommandPool = pool;


        init_info.Instance = renderAPI->GetInstance()->GetVKInstance();
        init_info.PhysicalDevice = renderAPI->GetPhysicalDevice()->GetPhysicalDevice();
        init_info.Device = renderAPI->GetDevice()->GetVulkanDevice();
        init_info.QueueFamily = renderAPI->GetPhysicalDevice()->GetFamilyIndices().GraphicsFamily.value();
        init_info.Queue = renderAPI->GetDevice()->GetGraphicsQueue();
        init_info.DescriptorPool = std::any_cast<vk::DescriptorPool>(s_Data.m_CommandPool);
        init_info.MinImageCount = 2;
        init_info.ImageCount = 3;
        init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        init_info.CheckVkResultFn = check_vk_result;
        init_info.Subpass = 0;


        ImGui_ImplVulkan_Init(&init_info, s_Data.m_RenderPass->GetHandle());

        // Upload Fonts
        {
            Ref<VulkanCommandBufferSet> uploadList = CommandBufferSet::Create({ 1 }).As<VulkanCommandBufferSet>();
            auto cmd = uploadList->GetCommandBufferAt(0);


            vk::CommandBuffer cmdbuffer = std::any_cast<vk::CommandBuffer>(cmd->GetHandle());
            VkCommandBuffer command_buffer = cmdbuffer;

            VkCommandBufferBeginInfo begin_info = {};
            begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
            VkResult  err = vkBeginCommandBuffer(command_buffer, &begin_info);
            check_vk_result(err);

            ImGui_ImplVulkan_CreateFontsTexture(command_buffer);

            VkSubmitInfo end_info = {};
            end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            end_info.commandBufferCount = 1;
            end_info.pCommandBuffers = &command_buffer;
            err = vkEndCommandBuffer(command_buffer);
            check_vk_result(err);
            err = vkQueueSubmit(renderAPI->GetDevice()->GetGraphicsQueue(), 1, &end_info, VK_NULL_HANDLE);
            check_vk_result(err);

            err = vkDeviceWaitIdle(renderAPI->GetDevice()->GetVulkanDevice());
            check_vk_result(err);
            ImGui_ImplVulkan_DestroyFontUploadObjects();

            uploadList->Destroy(renderAPI->GetDevice()->GetVulkanDevice());
        }

    }

    void Imgui::DestroyVulkanRenderer()
    {
        VkRenderAPI::GetVulkanDevice().destroyDescriptorPool(std::any_cast<vk::DescriptorPool>(s_Data.m_CommandPool));
        ImGui_ImplVulkan_Shutdown();
        
    }

    void Imgui::RecreateVulkanRenderer()
    {
        DestroyVulkanRenderer();
        InitVulkanRenderer();
    }

    void Imgui::Begin(const Ref<CommandBufferSet>& cmdList)
    {
        s_Data.m_CommandList = cmdList.As<VulkanCommandBufferSet>();

        ImGui_ImplGlfw_NewFrame();
    	ImGui_ImplVulkan_NewFrame();

        
   
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();

    }

    void Imgui::End()
	{

        ImGui::Render();

        auto cmd = s_Data.m_CommandList;
        auto frame = Renderer::GetCurrentFrame();
        auto cmdBuffer = cmd->GetCommandBufferAt(frame);
        VkCommandBuffer command_buffer = std::any_cast<vk::CommandBuffer>(cmdBuffer->GetHandle());
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), command_buffer);
       
        if ((*s_Data.io).ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
    }

    void Imgui::ShutDown()
    {
        
        auto type = RenderAPI::Get()->GetRenderAPIType();

        if (type == RenderAPIType::Vulkan)
        {
           
            auto renderAPI = dynamic_cast<VkRenderAPI*>(RenderAPI::Get());
            auto device = renderAPI->GetDevice()->GetVulkanDevice();
            auto result = device.waitIdle();
            vk::resultCheck(result, "Failed to wait");
            // s_Data.m_CommandList->Destroy(device);
            device.destroyDescriptorPool(std::any_cast<vk::DescriptorPool>(s_Data.m_CommandPool));
            
            DestroyVulkanRenderer();
           
        }
       
        ImGui_ImplGlfw_Shutdown();
        
        
        ImGui::DestroyContext();
    }
}
