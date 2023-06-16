#include "boidpch.h"

#include "RenderCommand.h"

#include <spdlog/spdlog.h>

#include "Buffer.h"
#include "BufferSet.h"
#include "CommandBufferSet.h"
#include "Framebuffer.h"
#include "GraphicsPipeline.h"
#include "RenderAPI.h"
#include "GraphicsSyncObjects.h"
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
	Unique<RendererStorage> RenderCommand::s_Data = nullptr;


	//TODO: Manage lifetimes of these


	Ref<Swapchain> RenderCommand::GetSwapChain()
	{
		return s_Data->m_Swapchain;
	}


	void RenderCommand::Init(RenderAPI* context, const ApplicationSettings& appSettings)
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


	void RenderCommand::BeginDraw(const Ref<Camera>& camera)
	{
	}

	void RenderCommand::SetMaxFramesInFlight(uint32_t frames)
	{
		s_Data->m_MaxFramesInFlight = frames;
	}

	uint32_t RenderCommand::GetMaxFramesInFlight()
	{
		return s_Data->m_MaxFramesInFlight;
	}

	uint32_t& RenderCommand::GetCurrentFrame()
	{
		return s_Data->m_CurrentFrame;
	}

	void RenderCommand::SetCurrentFrame(uint32_t currentFrame)
	{
		s_Data->m_CurrentFrame = currentFrame;
		
	}

	void RenderCommand::EndDraw()
	{
	}

	void RenderCommand::BeginCommands(const Ref<CommandBufferSet>& cmdLists, uint32_t index)
	{

		s_Data->m_CurrentCommandList = cmdLists;
		SetCurrentCommandBuffer(index);
		GetCurrentCommandBuffer()->Begin();
	}

	void RenderCommand::BeginCommandBuffer(const Ref<CommandBuffer>& cmdBuffer)
	{
		cmdBuffer->Begin();
	
	}

	void RenderCommand::BeginFrameCommands(const Ref<CommandBufferSet>& cmdList)
	{
		s_Data->m_CurrentCommandList = cmdList;
		SetCurrentCommandBuffer(GetGraphicsBackend()->GetCurrentImageIndex());
		GetCurrentCommandBuffer()->Begin();
	}

	void RenderCommand::EndFrameCommands()
	{
		GetCurrentCommandBuffer()->End();
	}

	void RenderCommand::EndCommandBuffer(const Ref<CommandBuffer>& cmdBuffer)
	{
		cmdBuffer->End();
	}

	void RenderCommand::EndCommands()
	{
		GetCurrentCommandBuffer()->End();
	}

	void RenderCommand::AcquireImageIndex()
	{
		GetGraphicsBackend()->GetSwapchainImageIndex(GetCurrentFrame());
	}


	void RenderCommand::BeginSwapChainRenderPass()
	{
		GetCurrentCommandBuffer()->BeginRenderPass(GetSwapChain()->GetRenderPass(),
		                                           GetSwapChain()->GetCurrentFrameBuffer());
	}

	void RenderCommand::EndSwapChainRenderPass()
	{
		GetCurrentCommandBuffer()->EndRenderPass();
	}

	Ref<CommandBuffer> RenderCommand::GetCurrentCommandBuffer()
	{
		
		
		return s_Data->m_CommandBuffer;
	}

	void RenderCommand::SetCurrentCommandBuffer(uint32_t currentFrame)
	{
		s_Data->m_CommandBuffer = s_Data->m_CurrentCommandList->GetCommandBufferAt(currentFrame);
	}

	bool RenderCommand::IsGraphicsBackendReady()
	{
		return s_Data->m_GraphicsBackend->IsReady();
	}

	Ref<GraphicsBackend> RenderCommand::GetGraphicsBackend()
	{
		return s_Data->m_GraphicsBackend;
	}


	void RenderCommand::Clear(ClearSettings settings)
	{
		GetSwapChain()->GetRenderPass()->Clear(settings);
	}

	void RenderCommand::DrawIndexed(uint32_t count, const PrimitiveType& primitive)
	{
		GetCurrentCommandBuffer()->DrawIndexed(count, IndexDataType::UnsignedInt);
	}

	void RenderCommand::DrawArrays(uint32_t vertexCount, const PrimitiveType& primitive)
	{
		GetCurrentCommandBuffer()->DrawArrays(vertexCount);
	}

	void RenderCommand::SetPipelineState(const Ref<GraphicsPipeline>& pipelineState)
	{
		s_Data->m_GraphicsBackend->SubmitPipeline(pipelineState);
		GetCurrentCommandBuffer()->BindGraphicsPipeline(pipelineState);
	}

	void RenderCommand::BeginRenderPass(const Ref<RenderPass>& renderPass, const Ref<Framebuffer>& buffer)
	{
		GetCurrentCommandBuffer()->BeginRenderPass(renderPass, buffer);
	}


	void RenderCommand::BeginRenderPass(const Ref<RenderPass>& renderPass, const std::vector<Ref<Framebuffer>>& buffers)
	{
		GetCurrentCommandBuffer()->BeginRenderPass(renderPass, buffers.at(GetCurrentFrame()));
	}

	void RenderCommand::BeginRenderPass(const Ref<RenderPass>& renderPass, const Ref<FrameBufferSet>& buffers)
	{
		GetCurrentCommandBuffer()->BeginRenderPass(renderPass,
			buffers->Get(GetCurrentFrame()));
	}

	void RenderCommand::BeginRenderPass(const Ref<Swapchain>& swapchain)
	{
		RenderCommand::BeginRenderPass(swapchain->GetRenderPass(),
			RenderCommand::GetSwapChain()->GetFrameBuffer(GetGraphicsBackend()->GetCurrentImageIndex()));
	}

	void RenderCommand::RegisterFreeFunc(const GraphicsBackend::RenderBackendFreeFunc& freeFunc)
	{
		GetGraphicsBackend()->RegisterResizeFunc(freeFunc);
	}

	void RenderCommand::BindGraphicsPipeline(const Ref<GraphicsPipeline>& pipeline)
	{
		s_Data->m_GraphicsBackend->SubmitPipeline(pipeline);
		GetCurrentCommandBuffer()->BindGraphicsPipeline(pipeline);
	}

	void RenderCommand::BindGraphicsDescriptorSets(uint32_t set, const std::vector<Ref<PipelineDescriptorSet>>& sets)
	{
		GetCurrentCommandBuffer()->BindGraphicsDescriptorSet(set, sets.at(GetCurrentFrame()));
	}

	void RenderCommand::EndRenderPass()
	{
		GetCurrentCommandBuffer()->EndRenderPass();
	}


	void RenderCommand::BindVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		GetCurrentCommandBuffer()->BindVertexBuffer(vertexBuffer);
	}

	void RenderCommand::BindVertexBuffer(const Ref<VertexBufferSet>& vertexBuffer)
	{
		GetCurrentCommandBuffer()->BindVertexBuffer(vertexBuffer->Get(GetCurrentFrame()));
	}

	void RenderCommand::BindIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		GetCurrentCommandBuffer()->BindIndexBuffer(indexBuffer);
	}

	void RenderCommand::SetViewport(const Viewport& viewport)
	{
		GetCurrentCommandBuffer()->SetViewPort(viewport);
	}

	void RenderCommand::SetScissor(const Rect& rect)
	{
		GetCurrentCommandBuffer()->SetScissor(rect);
	}

	void RenderCommand::PushCommandBufferSet(const Ref<CommandBufferSet>& commandBuffer)
	{
		s_Data->m_CurrentCommandLists.emplace_back(commandBuffer);
	}

	void RenderCommand::PushCommandBufferSets(const std::vector<Ref<CommandBufferSet>>& commandBuffers)
	{
		s_Data->m_CurrentCommandLists.insert(s_Data->m_CurrentCommandLists.end(), commandBuffers.begin(), commandBuffers.end());
	}


	void RenderCommand::SetStagingBufferData(const std::vector<Ref<StagingBuffer>>& buffers, const void* data)
	{
		buffers.at(GetCurrentFrame())->SetData(data);
	}

	void RenderCommand::CopyStagingBuffer(const std::vector<Ref<StagingBuffer>>& stagingBuffers,
		const std::vector<Ref<UniformBuffer>>& buffers)
	{
		GetCurrentCommandBuffer()->CopyUniformBuffer(stagingBuffers.at(GetCurrentFrame()), buffers.at(GetCurrentFrame()));
	}

	void RenderCommand::CopyStagingBuffer(const std::vector<Ref<StagingBuffer>>& stagingBuffers,
		const std::vector<Ref<StorageBuffer>>& buffers)
	{
		GetCurrentCommandBuffer()->CopyStorageBuffer(stagingBuffers.at(GetCurrentFrame()), buffers.at(GetCurrentFrame()));
	}

	void RenderCommand::SetUniformBufferData(const Ref<UniformBufferSet>& buffers, const void* data, uint32_t dataSize)
	{
		buffers->Get(GetCurrentFrame())->SetData(data, dataSize);
	}

	void RenderCommand::SetStagingBufferData(const Ref<StagingBufferSet>& buffers, const void* data)
	{
		buffers->Get(GetCurrentFrame())->SetData(data);
	}

	void RenderCommand::SetStagingBufferData(const Ref<StagingBufferSet>& buffers, const void* data, uint32_t dataSize)
	{
		buffers->Get(GetCurrentFrame())->SetData(data, dataSize);
	}

	void RenderCommand::CopyStagingBuffer(const Ref<StagingBufferSet>& stagingBuffers, const Ref<UniformBufferSet>& buffers)
	{
		GetCurrentCommandBuffer()->CopyUniformBuffer(stagingBuffers->Get(GetCurrentFrame()), buffers->Get(GetCurrentFrame()));
	}

	void RenderCommand::CopyStagingBuffer(const Ref<StagingBufferSet>& stagingBuffers, const Ref<StorageBufferSet>& buffers)
	{
		GetCurrentCommandBuffer()->CopyStorageBuffer(stagingBuffers->Get(GetCurrentFrame()), buffers->Get(GetCurrentFrame()));
	}

	void RenderCommand::CopyStagingBuffer(const Ref<StagingBufferSet>& stagingBuffers,
		const Ref<VertexBufferSet>& buffers)
	{
		GetCurrentCommandBuffer()->CopyVertexBuffer(stagingBuffers->Get(GetCurrentFrame()), buffers->Get(GetCurrentFrame()));
	}

	void RenderCommand::LineWidth(float lineWidth)
	{
		GetCurrentCommandBuffer()->SetLineWidth(lineWidth);
	}

	void RenderCommand::WaitForSubmitQueue()
	{
		GetGraphicsBackend()->WaitForQueue();
	}


	void RenderCommand::VertexShaderPushConstants(const Ref<GraphicsPipeline>& pipelineState, const void* data,
	                                              uint32_t dataSize, uint32_t offset)
	{
		GetCurrentCommandBuffer()->GraphicsPushConstant(pipelineState, ShaderType::Vertex,data, dataSize, offset);
	}

	void RenderCommand::FragmentShaderPushConstants(const Ref<GraphicsPipeline>& pipelineState, const void* data,
		uint32_t dataSize, uint32_t offset)
	{
		GetCurrentCommandBuffer()->GraphicsPushConstant(pipelineState, ShaderType::Fragment, data, dataSize, offset);
	}


	Ref<RenderPass> RenderCommand::GetDefaultRenderTarget()
	{
		return s_Data->m_CurrentRenderPass;
	}

	Ref<GraphicsPipeline> RenderCommand::GetDefaultPipeline()
	{
		return s_Data->m_DefaultPipelineState;
	}

	void RenderCommand::Resize(uint32_t width, uint32_t height)
	{
		
		//std::this_thread::sleep_for(std::chrono_literals::operator ""ms(200ull));
		//s_Data->m_CanSubmitToQueue = true;
		
		//s_Data->m_CanSubmitToQueue = true;

	}

	void RenderCommand::WaitAndRender()
	{
		GetGraphicsBackend()->SubmitGraphicsWork(s_Data->m_CurrentCommandLists);
	
	}

	void RenderCommand::PresentImage()
	{

		GetGraphicsBackend()->Present();

		const auto& maxFrames = RenderCommand::GetMaxFramesInFlight();
		auto frame = GetCurrentFrame();
		frame = (frame + 1) % maxFrames;
		SetCurrentFrame(frame);
	}
}
