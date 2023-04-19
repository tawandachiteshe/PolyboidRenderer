#include "boidpch.h"

#include "Renderer.h"

#include "Framebuffer.h"
#include "PipelineState.h"
#include "RenderAPI.h"
#include "RenderPass.h"
#include "CommandList/RenderCommand.h"
#include "CommandList/Commands/RenderCommands.h"
#include "Engine/Engine/Engine.h"


namespace Polyboid
{

	Unique<RendererStorage> Renderer::s_Data = nullptr;


    //TODO: Manage lifetimes of these

    void Renderer::Init(RenderAPI* context) {
      
        RenderCommand::Init(context);
		s_Data = std::make_unique<RendererStorage>();
        s_Data->m_Context = context;

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

    void Renderer::BeginCommands(const std::vector<Ref<CommandList>>& cmdList)
    {
        RenderCommand::SetCommandLists(cmdList);
        RenderCommand::AddCommand(ALLOC_COMMAND(BeginRenderCommand));
    }

    void Renderer::EndCommands()
    {
        RenderCommand::AddCommand(ALLOC_COMMAND(EndRenderCommand));
    }

    void Renderer::BeginFrame()
    {
       
    }

    void Renderer::EndFrame()
    {
        auto& frame = s_Data->m_CurrentFrame;
        auto& maxFrames = s_Data->m_MaxFramesInFlight;
        frame += frame % maxFrames;
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

    void Renderer::SubmitSwapChain(const Ref<Swapchain>& swapchain)
    {
        RenderCommand::AddCommand(ALLOC_COMMAND(SubmitSwapChainCommand, swapchain));
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
		RenderCommand::WaitAndRender();
    }
}
