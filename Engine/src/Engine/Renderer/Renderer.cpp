#include "boidpch.h"

#include "Renderer.h"

#include <spdlog/spdlog.h>

#include "Buffer.h"
#include "BufferSet.h"
#include "CommandBufferSet.h"
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
#include "Platform/Vulkan/VulkanGraphicsBackend.h"
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

		
		s_Data->m_GraphicsBackend = CreateRef<VulkanGraphicsBackend>().As<GraphicsBackend>();
		
	}


	void Renderer::BeginDraw(const Ref<Camera>& camera)
	{
	}

	void Renderer::SetMaxFramesInFlight(uint32_t frames)
	{
		s_Data->m_MaxFramesInFlight = frames;
	}

	uint32_t Renderer::GetMaxFramesInFlight()
	{
		return s_Data->m_MaxFramesInFlight;
	}

	uint32_t& Renderer::GetCurrentFrame()
	{
		return s_Data->m_CurrentFrame;
	}

	void Renderer::SetCurrentFrame(uint32_t currentFrame)
	{
		s_Data->m_CurrentFrame = currentFrame;
		
	}

	void Renderer::EndDraw()
	{
	}

	void Renderer::BeginCommands(const Ref<CommandBufferSet>& cmdLists, uint32_t index)
	{

		s_Data->m_CurrentCommandList = cmdLists;
		SetCurrentCommandBuffer(index);
		GetCurrentCommandBuffer()->Begin();
	}

	void Renderer::BeginCommandBuffer(const Ref<CommandBuffer>& cmdBuffer)
	{
		cmdBuffer->Begin();
	
	}

	void Renderer::BeginFrameCommands(const Ref<CommandBufferSet>& cmdList)
	{
		s_Data->m_CurrentCommandList = cmdList;
		SetCurrentCommandBuffer(GetGraphicsBackend()->GetCurrentImageIndex());
		GetCurrentCommandBuffer()->Begin();
	}

	void Renderer::EndFrameCommands()
	{
		GetCurrentCommandBuffer()->End();
	}

	void Renderer::EndCommandBuffer(const Ref<CommandBuffer>& cmdBuffer)
	{
		cmdBuffer->End();
	}

	void Renderer::EndCommands()
	{
		GetCurrentCommandBuffer()->End();
	}

	void Renderer::AcquireImageIndex()
	{
		GetGraphicsBackend()->GetSwapchainImageIndex(GetCurrentFrame());
	}


	void Renderer::BeginSwapChainRenderPass()
	{
		GetCurrentCommandBuffer()->BeginRenderPass(GetSwapChain()->GetRenderPass(),
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

	void Renderer::SetCurrentCommandBuffer(uint32_t currentFrame)
	{
		s_Data->m_CommandBuffer = s_Data->m_CurrentCommandList->GetCommandBufferAt(currentFrame);
	}

	bool Renderer::IsGraphicsBackendReady()
	{
		return s_Data->m_GraphicsBackend->IsReady();
	}

	Ref<GraphicsBackend> Renderer::GetGraphicsBackend()
	{
		return s_Data->m_GraphicsBackend;
	}


	void Renderer::Clear(ClearSettings settings)
	{
		GetSwapChain()->GetRenderPass()->Clear(settings);
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
		s_Data->m_GraphicsBackend->SubmitPipeline(pipelineState);
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

	void Renderer::BeginRenderPass(const Ref<RenderPass>& renderPass, const Ref<FrameBufferSet>& buffers)
	{
		GetCurrentCommandBuffer()->BeginRenderPass(renderPass,
			buffers->Get(GetCurrentFrame()));
	}

	void Renderer::BeginRenderPass(const Ref<Swapchain>& swapchain)
	{
		Renderer::BeginRenderPass(swapchain->GetRenderPass(),
			Renderer::GetSwapChain()->GetFrameBuffer(GetGraphicsBackend()->GetCurrentImageIndex()));
	}

	void Renderer::RegisterFreeFunc(const GraphicsBackend::RenderBackendFreeFunc& freeFunc)
	{
		GetGraphicsBackend()->RegisterResizeFunc(freeFunc);
	}

	void Renderer::BindGraphicsPipeline(const Ref<PipelineState>& pipeline)
	{
		s_Data->m_GraphicsBackend->SubmitPipeline(pipeline);
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

	void Renderer::PushCommandBufferSet(const Ref<CommandBufferSet>& commandBuffer)
	{
		s_Data->m_CurrentCommandLists.emplace_back(commandBuffer);
	}

	void Renderer::PushCommandBufferSets(const std::vector<Ref<CommandBufferSet>>& commandBuffers)
	{
		s_Data->m_CurrentCommandLists.insert(s_Data->m_CurrentCommandLists.end(), commandBuffers.begin(), commandBuffers.end());
	}

	void Renderer::SetUniformBufferData(const std::vector<Ref<UniformBuffer>>& buffers, const void* data,
	                                    uint32_t dataSize)
	{
		buffers.at(GetCurrentFrame())->SetData(data, dataSize);
	}

	void Renderer::SetStagingBufferData(const std::vector<Ref<StagingBuffer>>& buffers, const void* data)
	{
		buffers.at(GetCurrentFrame())->SetData(data);
	}

	void Renderer::CopyStagingBuffer(const std::vector<Ref<StagingBuffer>>& stagingBuffers,
		const std::vector<Ref<UniformBuffer>>& buffers)
	{
		GetCurrentCommandBuffer()->CopyUniformBuffer(stagingBuffers.at(GetCurrentFrame()), buffers.at(GetCurrentFrame()));
	}

	void Renderer::CopyStagingBuffer(const std::vector<Ref<StagingBuffer>>& stagingBuffers,
		const std::vector<Ref<StorageBuffer>>& buffers)
	{
		GetCurrentCommandBuffer()->CopyStorageBuffer(stagingBuffers.at(GetCurrentFrame()), buffers.at(GetCurrentFrame()));
	}

	void Renderer::SetUniformBufferData(const Ref<UniformBufferSet>& buffers, const void* data, uint32_t dataSize)
	{
		buffers->Get(GetCurrentFrame())->SetData(data, dataSize);
	}

	void Renderer::SetStagingBufferData(const Ref<StagingBufferSet>& buffers, const void* data)
	{
		buffers->Get(GetCurrentFrame())->SetData(data);
	}

	void Renderer::CopyStagingBuffer(const Ref<StagingBufferSet>& stagingBuffers, const Ref<UniformBufferSet>& buffers)
	{
		GetCurrentCommandBuffer()->CopyUniformBuffer(stagingBuffers->Get(GetCurrentFrame()), buffers->Get(GetCurrentFrame()));
	}

	void Renderer::CopyStagingBuffer(const Ref<StagingBufferSet>& stagingBuffers, const Ref<StorageBufferSet>& buffers)
	{
		GetCurrentCommandBuffer()->CopyStorageBuffer(stagingBuffers->Get(GetCurrentFrame()), buffers->Get(GetCurrentFrame()));
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

	void Renderer::Resize(uint32_t width, uint32_t height)
	{
		
		//std::this_thread::sleep_for(std::chrono_literals::operator ""ms(200ull));
		//s_Data->m_CanSubmitToQueue = true;
		
		//s_Data->m_CanSubmitToQueue = true;

	}

	void Renderer::WaitAndRender()
	{
		s_Data->m_GraphicsBackend->SubmitGraphicsWork(s_Data->m_CurrentCommandLists);
	}
}
