#pragma once
#include "Camera3D.h"
#include "RenderAPI.h"
#include "Shader.h"
#include "Engine/Engine/Base.h"
#include "Engine/Engine/MeshImporter/MeshImporter.h"
#include <Engine/Renderer/RenderPass.h>

#include "imgui.h"

namespace Polyboid
{
	class GraphicsBackend;
	class FrameBufferSet;
	class StorageBufferSet;
	class StagingBufferSet;
	class UniformBufferSet;
	struct Rect;
	struct Viewport;
	class PipelineDescriptorSet;
	class LayerContainer;
	class RendererSyncObjects;
	struct ApplicationSettings;
	class Fence;
	class Material;
	class UniformBuffer;
	class RenderAPI;
	

	struct RendererStorage
    {
        Ref<RenderPass> m_CurrentRenderPass;
        Ref<PipelineState> m_DefaultPipelineState;
        Ref<Swapchain> m_Swapchain;
        Ref<CommandBufferSet> m_CurrentCommandList;
        Ref<CommandBuffer> m_CommandBuffer;

        RendererStorage() = default;
		RenderAPI* m_Context = nullptr;
        uint32_t m_CurrentFrame = 0;
        uint32_t m_MaxFramesInFlight = 3;
        uint32_t m_ImageIndex = 0;
        Ref<RendererSyncObjects> m_CurrentSyncObjects;
        Ref<PipelineState> m_CurrentPipeline;
        bool m_IsMainRenderPass = false;
        Ref<GraphicsBackend> m_GraphicsBackend;
    };
	
    
    class Renderer
    {
    private:

        static Unique<RendererStorage> s_Data;
	 

    public:
        static Ref<Swapchain> GetSwapChain();

    	static void Init(RenderAPI* context, const ApplicationSettings& settings);
        static void BeginDraw(const Ref<Camera>& camera);
        static void SetMaxFramesInFlight(uint32_t frames);
        static uint32_t GetMaxFramesInFlight();
        static uint32_t& GetCurrentFrame();
        static void SetCurrentFrame(uint32_t currentFrame);
        static void EndDraw();
        static void BeginCommands(const Ref<CommandBufferSet>& cmdList, uint32_t index);
        static void BeginCommandBuffer(const Ref<CommandBuffer>& cmdBuffer);
        static void BeginFrameCommands(const Ref<CommandBufferSet>& cmdList);
        static void EndFrameCommands();
        static void EndCommandBuffer(const Ref<CommandBuffer>& cmdBuffer);
    	static void EndCommands();
        static void AcquireImageIndex();
        static void BeginSwapChainRenderPass();
        static void EndSwapChainRenderPass();
        static Ref<CommandBuffer> GetCurrentCommandBuffer();
        static void SetCurrentCommandBuffer(uint32_t currentFrame);
        static bool IsGraphicsBackendReady();
        static Ref<GraphicsBackend> GetGraphicsBackend();

        static void Clear(ClearSettings settings = {});

        static void DrawIndexed(uint32_t count, const PrimitiveType& primitive = PrimitiveType::Triangles);
        static void DrawArrays(uint32_t vertexCount, const PrimitiveType& primitive = PrimitiveType::Lines);
        static void SetPipelineState(const Ref<PipelineState>& pipelineState);
        static void BeginRenderPass(const Ref<RenderPass>& renderPass, const Ref<Framebuffer>& buffer);
        static void BeginRenderPass(const Ref<RenderPass>& renderPass, const std::vector<Ref<Framebuffer>>& buffers);
        static void BeginRenderPass(const Ref<RenderPass>& renderPass, const Ref<FrameBufferSet>& buffers);


        static void BindGraphicsPipeline(const Ref<PipelineState>& pipeline);
        static void BindGraphicsDescriptorSets(uint32_t set, const std::vector<Ref<PipelineDescriptorSet>>& sets);
        static void EndRenderPass();
        static void BindVertexBuffer(const Ref<VertexBuffer>& vertexBuffer);
        static void BindIndexBuffer(const Ref<IndexBuffer>& indexBuffer);
        static void SetViewport(const Viewport& viewport);
        static void SetScissor(const Rect& rect);

    	static void SetUniformBufferData(const std::vector<Ref<UniformBuffer>>& buffers, const void* data, uint32_t dataSize);
        static void SetStagingBufferData(const std::vector<Ref<StagingBuffer>>& buffers, const void* data);
        static void CopyStagingBuffer(const std::vector<Ref<StagingBuffer>>& stagingBuffers, const std::vector<Ref<UniformBuffer>>& buffers);
        static void CopyStagingBuffer(const std::vector<Ref<StagingBuffer>>& stagingBuffers, const std::vector<Ref<StorageBuffer>>& buffers);

        static void SetUniformBufferData(const Ref<UniformBufferSet>& buffers, const void* data, uint32_t dataSize);
        static void SetStagingBufferData(const Ref<StagingBufferSet>& buffers, const void* data);
        static void CopyStagingBuffer(const Ref<StagingBufferSet>& stagingBuffers, const Ref<UniformBufferSet>& buffers);
        static void CopyStagingBuffer(const Ref<StagingBufferSet>& stagingBuffers, const Ref<StorageBufferSet>& buffers);


        static void VertexShaderPushConstants(const Ref<PipelineState>& pipelineState, const void* data, uint32_t dataSize, uint32_t offset = 0);
        static void FragmentShaderPushConstants(const Ref<PipelineState>& pipelineState, const void* data, uint32_t dataSize, uint32_t offset = 0);

        static Ref<RenderPass> GetDefaultRenderTarget();
        static Ref<PipelineState> GetDefaultPipeline();

        static void Resize(uint32_t width, uint32_t height);

        static void WaitAndRender(const std::vector<Ref<CommandBufferSet>>& commandBuffers);
    };

}

