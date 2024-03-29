#include "boidpch.h"
#include "RenderCommands.h"

#include "Engine/Renderer/PipelineState.h"
#include "Engine/Renderer/VertexBufferArray.h"


namespace Polyboid
{
	void Command::SetContext(const Ref<RenderAPI>& context)
	{
		m_Context = context;
	}

	ClearRenderTargetCommand::ClearRenderTargetCommand(const Ref<RenderTarget>& renderTarget,
	                                                   const ClearSettings& clearSettings): m_State(renderTarget), m_ClearSettings(clearSettings)
	{
	}

	void ClearRenderTargetCommand::Execute()
	{
		m_State->Clear(m_ClearSettings);
	}

	DrawIndexedCommand::DrawIndexedCommand(uint32_t count, const PrimitiveType& type): m_Count(count), m_PrimitiveType(type)
	{
	}

	void DrawIndexedCommand::Execute()
	{

		m_Context->DrawIndexed(m_PrimitiveType, IndexDataType::UnsignedInt, m_Count);
	}

	DrawIndexedCommand::~DrawIndexedCommand()
	{
	}

	DrawArraysCommand::DrawArraysCommand(const PrimitiveType& type,
		uint32_t vertexCount): m_PrimitiveType(type), m_VertexCount(vertexCount)
	{
	}

	void DrawArraysCommand::Execute()
	{

		m_Context->DrawArrays(m_PrimitiveType, m_VertexCount);
	}


	PipelineStateCommand::PipelineStateCommand(const Ref<PipelineState>& pipeLine): m_State(pipeLine)
	{
	}

	void PipelineStateCommand::Execute()
	{
		m_State->Bind();
	}

	BeginRenderPassCommand::BeginRenderPassCommand(const Ref<RenderTarget>& renderTarget): m_RenderTarget(renderTarget)
	{
	}

	void BeginRenderPassCommand::Execute()
	{
		m_Context->BeginRenderPass(m_RenderTarget);
	}

	EndRenderPassCommand::EndRenderPassCommand(const Ref<RenderTarget>& renderTarget) : m_RenderTarget(renderTarget)
	{
	}

	void EndRenderPassCommand::Execute()
	{
		m_Context->EndRenderPass(m_RenderTarget);
	}
}
