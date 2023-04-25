#include "boidpch.h"

#include "Renderer.h"

#include "CommandList.h"
#include "Framebuffer.h"
#include "PipelineState.h"
#include "RenderAPI.h"
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

        for (uint32_t i = 0; i < s_Data->m_MaxFramesInFlight; ++i)
        {
            s_Data->m_InFlightFences.push_back(Fence::Create());
            s_Data->m_ImagesSemaphores.push_back(Semaphore::Create());
            s_Data->m_RenderSemaphores.push_back(Semaphore::Create());
        }


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

    void Renderer::BeginFrame()
    {
        auto& frame = s_Data->m_CurrentFrame;

        s_Data->m_InFlightFences[frame]->WaitAndReset();
        const auto& imageFence = s_Data->m_ImagesSemaphores[frame];
        s_Data->m_ImageIndex = s_Data->m_Swapchain->GetImageIndex(imageFence);
    }

    void Renderer::EndFrame()
    {
        auto& frame = s_Data->m_CurrentFrame;
        auto& inFlightFence = s_Data->m_InFlightFences[frame];
        auto& imageSemaphore = s_Data->m_ImagesSemaphores[frame];
        auto& renderSemaphore = s_Data->m_RenderSemaphores[frame];

        RenderCommand::WaitAndRender(imageSemaphore, renderSemaphore, inFlightFence);

        s_Data->m_Swapchain->Present(renderSemaphore);

        auto& maxFrames = s_Data->m_MaxFramesInFlight;
        frame = (frame + 1) % maxFrames;
    }

    void Renderer::ClearRenderPass(ClearSettings settings)
    {
        RenderCommand::AddCommand(ALLOC_COMMAND(ClearRenderPassCommand, s_Data->m_CurrentRenderPass, settings));
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


    void Renderer::BeginRenderPass(const Ref<RenderPass>& renderPass)
    {
        s_Data->m_CurrentRenderPass = renderPass;
        RenderCommand::AddCommand(ALLOC_COMMAND(BeginRenderPassCommand, s_Data->m_CurrentRenderPass));
    }

    void Renderer::EndRenderPass()
    {
        RenderCommand::AddCommand(ALLOC_COMMAND(EndRenderPassCommand, s_Data->m_CurrentRenderPass));
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
