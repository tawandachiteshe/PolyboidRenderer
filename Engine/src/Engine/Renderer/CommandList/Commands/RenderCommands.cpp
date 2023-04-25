#include "boidpch.h"
#include "RenderCommands.h"

#include <spdlog/fmt/bundled/core.h>

#include "Engine/Engine/Application.h"
#include "Engine/Renderer/CommandList.h"
#include "Engine/Renderer/PipelineState.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/VertexBufferArray.h"
#include "Platform/Vulkan/VkSwapChain.h"


namespace Polyboid
{
	void Command::SetContext(const Ref<CommandBuffer>& context)
	{
		m_Context = context;
	}

	ClearRenderTargetCommand::ClearRenderTargetCommand(const Ref<RenderPass>& renderTarget,
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

		//m_Context->DrawIndexed(m_PrimitiveType, IndexDataType::UnsignedInt, m_Count);
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

		//m_Context->DrawArrays(m_PrimitiveType, m_VertexCount);
	}


	PipelineStateCommand::PipelineStateCommand(const Ref<PipelineState>& pipeLine): m_State(pipeLine)
	{
	}

	void PipelineStateCommand::Execute()
	{
		m_State->Bind();
	}

	BeginRenderPassCommand::BeginRenderPassCommand(const Ref<RenderPass>& renderTarget): m_RenderTarget(renderTarget)
	{
	}

	void BeginRenderPassCommand::Execute()
	{
		auto vkSwapchain = std::reinterpret_pointer_cast<VkSwapChain>(Renderer::GetSwapChain());

		auto vkFramebuffer = vkSwapchain->GetCurrentFramebuffer();
		auto framebuffer = std::reinterpret_pointer_cast<Framebuffer>(vkFramebuffer);

		m_Context->BeginRenderPass(m_RenderTarget, framebuffer);
		//m_Context->BeginRenderPass(m_RenderTarget);
	}

	ClearRenderPassCommand::ClearRenderPassCommand(const Ref<RenderPass>& renderTarget, ClearSettings settings): m_RenderTarget(renderTarget), m_Settings(settings)
	{
	}

	void ClearRenderPassCommand::Execute()
	{
		m_RenderTarget->Clear(m_Settings);
	}

	EndRenderPassCommand::EndRenderPassCommand(const Ref<RenderPass>& renderTarget) : m_RenderTarget(renderTarget)
	{
	}

	void EndRenderPassCommand::Execute()
	{
		m_Context->EndRenderPass();
	}

	void BeginRenderCommand::Execute()
	{
		m_Context->Begin();
	}

	void RenderImguiCommand::Execute()
	{
		
	}

	void EndRenderCommand::Execute()
	{
		m_Context->End();
	}

	PushUniformBufferCommand::PushUniformBufferCommand(const Ref<PipelineState>& pipeLine, const std::string& name ,const void* data, uint32_t dataSize):
	m_State(pipeLine),  m_Data(data), m_DataSize(dataSize), m_Name(name)
	{
	}

	void PushUniformBufferCommand::Execute()
	{


		auto& shaders = m_State->GetShaders();

		for (auto& shaderType : shaders)
		{

			auto& [type, shader] = shaderType;

			if (const auto uniformBuffer = shader->GetUniformBuffer(m_Name))
			{
				uniformBuffer->SetData(m_Data, m_DataSize);
			}
		}


	}

}
