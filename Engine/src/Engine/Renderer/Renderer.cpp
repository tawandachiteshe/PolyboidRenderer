#include "boidpch.h"

#include "Renderer.h"

#include "Framebuffer.h"
#include "PipelineState.h"
#include "RenderAPI.h"
#include "RenderTarget.h"
#include "CommandList/RenderCommand.h"
#include "CommandList/Commands/RenderCommands.h"
#include "Engine/Engine/Engine.h"


namespace Polyboid
{

	Unique<RendererStorage> Renderer::s_Data = nullptr;


    //TODO: Manage lifetimes of these

    void Renderer::Init(const Ref<RenderAPI>& context) {
      
        RenderCommand::Init(context);
		s_Data = std::make_unique<RendererStorage>();
        


		//Add default render targets
		ClearSettings clearSettings;
		clearSettings.color = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
		RenderTargetSettings rtSettings;
        rtSettings.Width = 1600;
		rtSettings.Height = 900;
		rtSettings.TextureAttachments = {
			{ TextureAttachmentSlot::DepthStencil, TextureSizedInternalFormat::Depth24Stencil8 },
			{ TextureAttachmentSlot::Color0, TextureSizedInternalFormat::RGBA8 }
		};

        s_Data->m_DefaultRenderTarget = context->CreateRenderTarget(rtSettings);
		s_Data->m_DefaultPipelineState = context->CreatePipelineState();

    }
    
    
    void Renderer::BeginDraw(const Ref<Camera>& camera)
    {

    }

    void Renderer::EndDraw()
    {
       
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

    void Renderer::ClearDefaultRenderTarget(const ClearSettings& settings)
    {
        RenderCommand::AddCommand(ALLOC_COMMAND(ClearRenderTargetCommand, s_Data->m_DefaultRenderTarget, settings));
    }

    void Renderer::BeginDefaultRenderPass()
    {
        RenderCommand::AddCommand(ALLOC_COMMAND(BeginRenderPassCommand, s_Data->m_DefaultRenderTarget));
    }

    void Renderer::EndDefaultRenderPass()
    {
        RenderCommand::AddCommand(ALLOC_COMMAND(EndRenderPassCommand, s_Data->m_DefaultRenderTarget));
    }

    Ref<RenderTarget> Renderer::GetDefaultRenderTarget()
    {
        return s_Data->m_DefaultRenderTarget;
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
