#include "boidpch.h"
#include "RenderCommands.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/bundled/core.h>

#include "Engine/Engine/Application.h"
#include "Engine/Engine/ImguiSetup.h"
#include "Engine/Renderer/CommandList.h"
#include "Engine/Renderer/PipelineState.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/VertexBufferArray.h"
#include "Platform/Vulkan/VkSwapChain.h"
#include "imgui.h"


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

		m_Context->DrawIndexed(m_Count, IndexDataType::UnsignedInt);
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

		m_Context->DrawArrays(m_VertexCount);
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
		const auto framebuffer = m_RenderTarget->GetFramebuffer();

		m_Context->BeginRenderPass(m_RenderTarget, framebuffer);
		//m_Context->BeginRenderPass(m_RenderTarget);
	}


	void BeginSwapChainRenderPass::Execute()
	{

		auto vkSwapchain = std::reinterpret_pointer_cast<VkSwapChain>(Renderer::GetSwapChain());

		auto vkFramebuffer = vkSwapchain->GetCurrentFramebuffer();
		auto framebuffer = std::reinterpret_pointer_cast<Framebuffer>(vkFramebuffer);
		auto renderPass = vkSwapchain->GetDefaultRenderPass();

		m_Context->BeginRenderPass(renderPass, framebuffer);
	}



	void EndSwapChainRenderPass::Execute()
	{
		m_Context->EndRenderPass();
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

	BeginImguiRender::BeginImguiRender(const Ref<CommandList>& cmdList): m_CmdList(cmdList)
	{
	}

	void BeginImguiRender::Execute()
	{
		Imgui::Begin(m_CmdList);
	}

	TestCommand::TestCommand(ImTextureID ds): m_TestDs(ds)
	{
	}

	void TestCommand::Execute()
	{
		ImGui::Begin("Test from commands");
		ImGui::Text("Test card");
		ImGui::Image(m_TestDs, ImVec2(1024, 1024));
		ImGui::End();
	}

	RenderImGuiCommand::RenderImGuiCommand(const LayerContainer& container) : m_Container(container)
	{
	}

	void RenderImGuiCommand::Execute()
	{
		for (auto& layer : m_Container)
		{
			layer->OnImguiRender();
		}
	}

	EndImguiRender::EndImguiRender()
	{
	}

	void EndImguiRender::Execute()
	{
		Imgui::End();
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


	}

	BindGraphicsPipelineCommand::BindGraphicsPipelineCommand(const Ref<PipelineState>& pipeLine): m_State(pipeLine)
	{
	}

	void BindGraphicsPipelineCommand::Execute()
	{
		m_Context->BindGraphicsPipeline(m_State);
	}

	BindGraphicsPipelineDescSetsCommand::BindGraphicsPipelineDescSetsCommand(uint32_t setBinding, const Ref<PipelineDescriptorSet>& pipeLine): m_State(pipeLine), m_SetBinding(setBinding)
	{

	}

	void BindGraphicsPipelineDescSetsCommand::Execute()
	{
		m_Context->BindDescriptorSet(m_SetBinding, m_State);
	}

	BindVertexBufferCommand::BindVertexBufferCommand(const Ref<VertexBuffer>& buffer): m_Buffer(buffer)
	{
	}

	void BindVertexBufferCommand::Execute()
	{
		m_Context->BindVertexBuffer(m_Buffer);
	}

	BindIndexBufferCommand::BindIndexBufferCommand(const Ref<IndexBuffer>& buffer): m_Buffer(buffer)
	{
	}

	void BindIndexBufferCommand::Execute()
	{
		m_Context->BindIndexBuffer(m_Buffer);
	}

	SetViewportCommand::SetViewportCommand(const Viewport& viewport): m_Viewport(viewport)
	{

	}

	void SetViewportCommand::Execute()
	{
		m_Context->SetViewPort(m_Viewport);
	}

	SetScissorCommand::SetScissorCommand(const Rect& rect): m_Rect(rect)
	{
	}

	void SetScissorCommand::Execute()
	{
		m_Context->SetScissor(m_Rect);
	}
}
