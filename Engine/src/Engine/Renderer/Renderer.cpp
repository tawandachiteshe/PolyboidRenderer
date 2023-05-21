#include "boidpch.h"

#include "Renderer.h"

#include <spdlog/spdlog.h>

#include "Buffer.h"
#include "CommandList.h"
#include "Framebuffer.h"
#include "PipelineState.h"
#include "RenderAPI.h"
#include "RendererSyncObjects.h"
#include "RenderPass.h"
#include "SyncObjects.h"
#include "UniformBuffer.h"
#include "Engine/Engine/Application.h"
#include "Engine/Engine/Engine.h"
#include "Platform/Vulkan/VkRenderAPI.h"
#include "Platform/Vulkan/Utils/VulkanDevice.h"


namespace Polyboid
{
	Unique<RendererStorage> Renderer::s_Data = nullptr;


	//TODO: Manage lifetimes of these


	Ref<Swapchain> Renderer::GetSwapChain()
	{
		return s_Data->m_Swapchain;
	}

	void Renderer::Init(RenderAPI* context, const ApplicationSettings& appSettings)
	{
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

	uint32_t& Renderer::GetCurrentFrame()
	{
		return s_Data->m_CurrentFrame;
	}

	void Renderer::EndDraw()
	{
	}

	void Renderer::BeginCommands(const Ref<CommandList>& cmdLists)
	{
		s_Data->m_CurrentCommandList = cmdLists;
		cmdLists->GetCommandBufferAt(s_Data->m_ImageIndex)->Begin();
		SetCurrentCommandBuffer(cmdLists->GetCommandBufferAt(s_Data->m_ImageIndex));
	}

	void Renderer::BeginCommandBuffer(const Ref<CommandBuffer>& cmdBuffer)
	{
		cmdBuffer->Begin();
		SetCurrentCommandBuffer(cmdBuffer);
	}

	void Renderer::EndCommandBuffer(const Ref<CommandBuffer>& cmdBuffer)
	{
		cmdBuffer->End();
	}

	void Renderer::EndCommands()
	{
		s_Data->m_CurrentCommandList->GetCommandBufferAt(s_Data->m_ImageIndex)->End();
	}

	void Renderer::SubmitCommandList(const Ref<CommandList>& cmdList)
	{
		


		auto cmdBuffer = cmdList->GetCommandBufferAt(s_Data->m_ImageIndex);
		s_Data->m_CommandBuffers.emplace_back(cmdBuffer);

		
	}

	void Renderer::BeginFrame()
	{

		auto& frame = s_Data->m_CurrentFrame;
		const auto& imageFence = s_Data->m_CurrentSyncObjects->GetInFlightFence(frame);
		RenderAPI::Get()->WaitForFences(imageFence);

		const auto& imageSemaphore = s_Data->m_CurrentSyncObjects->GetImageSemaphore(frame);
		s_Data->m_ImageIndex = s_Data->m_Swapchain->GetImageIndex(imageSemaphore);
	}

	void Renderer::EndFrame()
	{
		auto frame = s_Data->m_CurrentFrame;

		const auto& syncObjects = s_Data->m_CurrentSyncObjects;
		const auto& renderSemaphore = syncObjects->GetRenderSemaphore(frame);
		const auto& inFlightFence = syncObjects->GetInFlightFence(frame);
		const auto& imageSemaphore = syncObjects->GetImageSemaphore(frame);

		s_Data->m_Context->SubmitCommandBuffer(s_Data->m_CommandBuffers, imageSemaphore, renderSemaphore, inFlightFence);

		s_Data->m_Swapchain->Present(renderSemaphore);

		auto maxFrames = s_Data->m_MaxFramesInFlight;
		frame = (frame + 1) % maxFrames;

		s_Data->m_CurrentFrame = frame;

		s_Data->m_CommandBuffers.clear();
	}

	void Renderer::DisplayImGuiTexture(ImTextureID ds)
	{
		// RenderCommand::AddCommand(ALLOC_COMMAND(class TestCommand, ds));
	}

	void Renderer::RenderImGui(const LayerContainer& layerContainer)
	{
		//RenderCommand::AddCommand(ALLOC_COMMAND(RenderImGuiCommand, layerContainer));
	}

	void Renderer::BeginImGui()
	{
		//RenderCommand::AddCommand(ALLOC_COMMAND(BeginImguiRender, s_Data->m_CurrentCommandList));
	}

	void Renderer::EndImGui()
	{
		//RenderCommand::AddCommand(ALLOC_COMMAND(EndImguiRender));
	}

	void Renderer::BeginSwapChainRenderPass()
	{
		GetCurrentCommandBuffer()->BeginRenderPass(GetSwapChain()->GetDefaultRenderPass(),
		                                           GetSwapChain()->GetCurrentFrameBuffer());
	}

	void Renderer::EndSwapChainRenderPass()
	{
		GetCurrentCommandBuffer()->EndRenderPass();
	}

	Ref<CommandBuffer> Renderer::GetCurrentCommandBuffer()
	{
		return s_Data->m_CommandBuffer;
	}

	void Renderer::SetCurrentCommandBuffer(const Ref<CommandBuffer>& cmdBuffer)
	{
		s_Data->m_CommandBuffer = cmdBuffer;
	}

	uint32_t Renderer::GetSwapChainImageIndex()
	{
		return s_Data->m_ImageIndex;
	}


	void Renderer::Clear(ClearSettings settings)
	{
		GetSwapChain()->GetDefaultRenderPass()->Clear(settings);
	}

	void Renderer::DrawIndexed(uint32_t count, const PrimitiveType& primitive)
	{
		GetCurrentCommandBuffer()->DrawIndexed(count, IndexDataType::UnsignedInt);
	}

	void Renderer::DrawArrays(uint32_t vertexCount, const PrimitiveType& primitive)
	{
		GetCurrentCommandBuffer()->DrawArrays(vertexCount);
	}

	void Renderer::SetPipelineState(const Ref<PipelineState>& pipelineState)
	{
		GetCurrentCommandBuffer()->BindGraphicsPipeline(pipelineState);
	}

	void Renderer::BeginRenderPass(const Ref<RenderPass>& renderPass, const Ref<Framebuffer>& buffer)
	{
		GetCurrentCommandBuffer()->BeginRenderPass(renderPass, buffer);
	}


	void Renderer::BeginRenderPass(const Ref<RenderPass>& renderPass, const std::vector<Ref<Framebuffer>>& buffers)
	{
		GetCurrentCommandBuffer()->BeginRenderPass(renderPass, buffers.at(GetCurrentFrame()));
	}

	void Renderer::BindGraphicsPipeline(const Ref<PipelineState>& pipeline)
	{
		GetCurrentCommandBuffer()->BindGraphicsPipeline(pipeline);
	}

	void Renderer::BindGraphicsDescriptorSets(uint32_t set, const std::vector<Ref<PipelineDescriptorSet>>& sets)
	{
		GetCurrentCommandBuffer()->BindDescriptorSet(set, sets.at(GetCurrentFrame()));
	}

	void Renderer::EndRenderPass()
	{
		GetCurrentCommandBuffer()->EndRenderPass();
	}


	void Renderer::BindVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		GetCurrentCommandBuffer()->BindVertexBuffer(vertexBuffer);
	}

	void Renderer::BindIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		GetCurrentCommandBuffer()->BindIndexBuffer(indexBuffer);
	}

	void Renderer::SetViewport(const Viewport& viewport)
	{
		GetCurrentCommandBuffer()->SetViewPort(viewport);
	}

	void Renderer::SetScissor(const Rect& rect)
	{
		GetCurrentCommandBuffer()->SetScissor(rect);
	}

	void Renderer::SetUniformBufferData(const std::vector<Ref<UniformBuffer>>& buffers, const void* data,
		uint32_t dataSize)
	{
		buffers.at(GetSwapChainImageIndex())->SetData(data, dataSize);
	}

	void Renderer::SetStagingBufferData(const std::vector<Ref<StagingBuffer>>& buffers, const void* data)
	{
		buffers.at(GetSwapChainImageIndex())->SetData(data);
	}

	void Renderer::CopyStagingBuffer(const std::vector<Ref<StagingBuffer>>& stagingBuffers,
		const std::vector<Ref<UniformBuffer>>& buffers)
	{
		GetCurrentCommandBuffer()->CopyUniformBuffer(stagingBuffers.at(GetSwapChainImageIndex()), buffers.at(GetSwapChainImageIndex()));
	}

	void Renderer::CopyStagingBuffer(const std::vector<Ref<StagingBuffer>>& stagingBuffers,
		const std::vector<Ref<StorageBuffer>>& buffers)
	{
		GetCurrentCommandBuffer()->CopyStorageBuffer(stagingBuffers.at(GetSwapChainImageIndex()), buffers.at(GetSwapChainImageIndex()));
	}
	

	void Renderer::VertexShaderPushConstants(const Ref<PipelineState>& pipelineState, const void* data,
	                                         uint32_t dataSize, uint32_t offset)
	{
		GetCurrentCommandBuffer()->PushConstant(pipelineState, ShaderType::Vertex,data, dataSize, offset);
	}

	void Renderer::FragmentShaderPushConstants(const Ref<PipelineState>& pipelineState, const void* data,
		uint32_t dataSize, uint32_t offset)
	{
		GetCurrentCommandBuffer()->PushConstant(pipelineState, ShaderType::Fragment, data, dataSize, offset);
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
