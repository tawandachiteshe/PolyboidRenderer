﻿#pragma once
#include "Camera3D.h"
#include "RenderAPI.h"
#include "Shader.h"
#include "Engine/Engine/Base.h"
#include "Engine/Engine/MeshImporter/MeshImporter.h"
#include <Engine/Renderer/RenderPass.h>

#include "imgui.h"

namespace Polyboid
{
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
        Ref<CommandList> m_CurrentCommandList;

        RendererStorage() = default;
		RenderAPI* m_Context = nullptr;
        std::atomic_uint32_t m_CurrentFrame = 0;
        uint32_t m_MaxFramesInFlight = 3;
        uint32_t m_ImageIndex = 0;
        Ref<RendererSyncObjects> m_CurrentSyncObjects;
        Ref<PipelineState> m_CurrentPipeline;
        bool m_IsMainRenderPass = false;
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
        static std::atomic_uint32_t& GetCurrentFrame();
        static void EndDraw();
        static void BeginCommands(const std::vector<Ref<CommandList>>& cmdList);
        static void EndCommands();
        static void SubmitCurrentCommandList();
        static void BeginFrame(const Ref<RendererSyncObjects>& syncObjects);
        static void EndFrame();
        static void DisplayImGuiTexture(ImTextureID ds);
        static void RenderImGui(const LayerContainer& layerContainer);
        static void BeginImGui();
        static void EndImGui();
        static void BeginSwapChainRenderPass();
        static void EndSwapChainRenderPass();

        static void Clear(ClearSettings settings = {});

        static void DrawIndexed(uint32_t count, const PrimitiveType& primitive = PrimitiveType::Triangles);
        static void DrawArrays(uint32_t vertexCount, const PrimitiveType& primitive = PrimitiveType::Lines);
        static void SetPipelineState(const Ref<PipelineState>& pipelineState);
        static void BeginRenderPass(const Ref<RenderPass>& renderPass, bool isMain = false);
        static void BeginRenderPass(const Ref<RenderPass>& renderPass, const std::vector<Ref<Framebuffer>>& buffers);
        static void BindGraphicsPipeline(const Ref<PipelineState>& pipeline);
        static void BindGraphicsDescriptorSets(uint32_t set, const std::vector<Ref<PipelineDescriptorSet>>& sets);
        static void EndRenderPass();
        static void BindVertexBuffer(const Ref<VertexBuffer>& vertexBuffer);
        static void BindIndexBuffer(const Ref<IndexBuffer>& indexBuffer);
        static void SetViewport(const Viewport& viewport);
        static void SetScissor(const Rect& rect);

        static Ref<RenderPass> GetDefaultRenderTarget();
        static Ref<PipelineState> GetDefaultPipeline();
        
        static void WaitAndRender();
    };

}

