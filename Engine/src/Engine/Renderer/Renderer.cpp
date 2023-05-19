#include "boidpch.h"

#include "Renderer.h"

#include "CommandList.h"
#include "Framebuffer.h"
#include "PipelineState.h"
#include "RenderAPI.h"
#include "RendererSyncObjects.h"
#include "RenderPass.h"
#include "SyncObjects.h"
#include "CommandList/RenderCommand.h"
#include "CommandList/Commands/RenderCommands.h"
#include "Engine/Engine/Application.h"
#include "Engine/Engine/Engine.h"


namespace Polyboid
{

	Unique<RendererStorage> Renderer::s_Data = nullptr;


    //TODO: Manage lifetimes of these


    Ref<Swapchain> Renderer::GetSwapChain()
    {
        return s_Data->m_Swapchain;
    }

    void Renderer::Init(RenderAPI* context, const ApplicationSettings& appSettings) {
      
        RenderCommand::Init(context);
		s_Data = std::make_unique<RendererStorage>();
        s_Data->m_Context = context;
        

        SwapchainSettings settings{};
        settings.Width = appSettings.WindowWidth;
        settings.Height = appSettings.WindowHeight;
        settings.SwapchainFormat = EngineGraphicsFormats::BGRA8ISrgb;

        s_Data->m_Swapchain = Swapchain::Create(settings);
        s_Data->m_CurrentSyncObjects = RendererSyncObjects::Create(s_Data->m_MaxFramesInFlight);



    }
    
    
    void Renderer::BeginDraw(const Ref<Camera>& camera)
    {

    }

    void Renderer::SetMaxFramesInFlight(uint32_t frames)
    {
        s_Data->m_MaxFramesInFlight = frames;
    }

    std::atomic_uint32_t& Renderer::GetCurrentFrame()
    {
        return s_Data->m_CurrentFrame;
    }

    void Renderer::EndDraw()
    {
       
    }

    void Renderer::BeginCommands(const std::vector<Ref<CommandList>>& cmdLists)
    {

        s_Data->m_CurrentCommandList = cmdLists.at(0);
        RenderCommand::SetCommandLists(cmdLists);
        // for (auto& cmdList : cmdLists)
        // {
        //     cmdList->GetCommandBufferAt(s_Data->m_CurrentFrame)->Reset();
        // }
    	RenderCommand::AddCommand(ALLOC_COMMAND(BeginRenderCommand));
      
        
    }

    void Renderer::EndCommands()
    {

      
        RenderCommand::AddCommand(ALLOC_COMMAND(EndRenderCommand));
    }

    void Renderer::SubmitCurrentCommandList()
    {
        auto& frame = s_Data->m_CurrentFrame;
        auto& syncObjects = s_Data->m_CurrentSyncObjects;

        const auto& inFlightFence = syncObjects->GetInFlightFence(frame);
        const auto& imageSemaphore = syncObjects->GetImageSemaphore(frame);
        const auto& renderSemaphore = syncObjects->GetRenderSemaphore(frame);

        RenderCommand::WaitAndRender(imageSemaphore, renderSemaphore, inFlightFence);

    }

    void Renderer::BeginFrame(const Ref<RendererSyncObjects>& syncObjects)
    {
        s_Data->m_CurrentSyncObjects = syncObjects;
        auto& frame = s_Data->m_CurrentFrame;

        const auto& imageFence = s_Data->m_CurrentSyncObjects->GetInFlightFence(frame);
    	imageFence->WaitAndReset();
        const auto& imageSemaphore = s_Data->m_CurrentSyncObjects->GetImageSemaphore(frame);
        s_Data->m_ImageIndex = s_Data->m_Swapchain->GetImageIndex(imageSemaphore);
    }

    void Renderer::EndFrame()
    {
        auto& frame = s_Data->m_CurrentFrame;

        const auto& syncObjects = s_Data->m_CurrentSyncObjects;
        const auto& renderSemaphore = syncObjects->GetRenderSemaphore(frame);


        s_Data->m_Swapchain->Present(renderSemaphore);

        auto& maxFrames = s_Data->m_MaxFramesInFlight;
        frame = (frame + 1) % maxFrames;
    }

    void Renderer::DisplayImGuiTexture(ImTextureID ds)
    {
        RenderCommand::AddCommand(ALLOC_COMMAND(class TestCommand, ds));
    }

    void Renderer::RenderImGui(const LayerContainer& layerContainer)
    {
        RenderCommand::AddCommand(ALLOC_COMMAND(RenderImGuiCommand, layerContainer));
    }

    void Renderer::BeginImGui()
    {
		RenderCommand::AddCommand(ALLOC_COMMAND(BeginImguiRender, s_Data->m_CurrentCommandList));
    }

    void Renderer::EndImGui()
    {
        RenderCommand::AddCommand(ALLOC_COMMAND(EndImguiRender));
    }

    void Renderer::BeginSwapChainRenderPass()
    {
        RenderCommand::AddCommand(ALLOC_COMMAND(class BeginSwapChainRenderPass));
    }

    void Renderer::EndSwapChainRenderPass()
    {
        RenderCommand::AddCommand(ALLOC_COMMAND(class EndSwapChainRenderPass));
    }

    void Renderer::Clear(ClearSettings settings)
    {
        RenderCommand::AddCommand(ALLOC_COMMAND(ClearRenderPassCommand, GetSwapChain()->GetDefaultRenderPass(), settings));
    }

    void Renderer::DrawIndexed(uint32_t count, const PrimitiveType& primitive)
    {
        RenderCommand::AddCommand(ALLOC_COMMAND(DrawIndexedCommand, count, primitive));
    }

    void Renderer::DrawArrays(uint32_t vertexCount, const PrimitiveType& primitive)
    {
        RenderCommand::AddCommand(ALLOC_COMMAND(DrawArraysCommand, primitive, vertexCount));
    }

    void Renderer::SetPipelineState(const Ref<PipelineState>& pipelineState)
    {
        RenderCommand::AddCommand(ALLOC_COMMAND(PipelineStateCommand, pipelineState));
    }


    void Renderer::BeginRenderPass(const Ref<RenderPass>& renderPass, bool isMain)
    {
        s_Data->m_CurrentRenderPass = renderPass;
        RenderCommand::AddCommand(ALLOC_COMMAND(BeginRenderPassCommand, s_Data->m_CurrentRenderPass));
        s_Data->m_IsMainRenderPass = isMain;


    }

    void Renderer::BeginRenderPass(const Ref<RenderPass>& renderPass, const std::vector<Ref<Framebuffer>>& buffers)
    {
        renderPass->SetFramebuffer(buffers[GetCurrentFrame()]);
        s_Data->m_CurrentRenderPass = renderPass;
        RenderCommand::AddCommand(ALLOC_COMMAND(BeginRenderPassCommand, s_Data->m_CurrentRenderPass));
    }

    void Renderer::BindGraphicsPipeline(const Ref<PipelineState>& pipeline)
    {
        RenderCommand::AddCommand(ALLOC_COMMAND(BindGraphicsPipelineCommand, pipeline));
    }

    void Renderer::BindGraphicsDescriptorSets(uint32_t set, const std::vector<Ref<PipelineDescriptorSet>>& sets)
    {
        RenderCommand::AddCommand(ALLOC_COMMAND(BindGraphicsPipelineDescSetsCommand, set, sets[GetCurrentFrame()]));
    }

    void Renderer::EndRenderPass()
    {

        RenderCommand::AddCommand(ALLOC_COMMAND(EndRenderPassCommand, s_Data->m_CurrentRenderPass));
    }

    void Renderer::BindVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
    {
        RenderCommand::AddCommand(ALLOC_COMMAND(BindVertexBufferCommand, vertexBuffer));
    }

    void Renderer::BindIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
    {
        RenderCommand::AddCommand(ALLOC_COMMAND(BindIndexBufferCommand, indexBuffer));
    }

    void Renderer::SetViewport(const Viewport& viewport)
    {
        RenderCommand::AddCommand(ALLOC_COMMAND(SetViewportCommand, viewport));
    }

    void Renderer::SetScissor(const Rect& rect)
    {
        RenderCommand::AddCommand(ALLOC_COMMAND(SetScissorCommand, rect));
    }


    Ref<RenderPass> Renderer::GetDefaultRenderTarget()
    {
        return s_Data->m_CurrentRenderPass;
    }

    Ref<PipelineState> Renderer::GetDefaultPipeline()
    {
        return s_Data->m_DefaultPipelineState;
    }

    void Renderer::WaitAndRender()
    {
		
    }
}
