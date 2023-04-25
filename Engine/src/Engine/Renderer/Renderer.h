#pragma once
#include "Camera3D.h"
#include "RenderAPI.h"
#include "Shader.h"
#include "VertexBufferArray.h"
#include "Engine/Engine/Base.h"
#include "Engine/Engine/MeshImporter/MeshImporter.h"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "glm/matrix.hpp"
#include <Engine/Renderer/RenderPass.h>

namespace Polyboid
{
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

        RendererStorage() = default;
		RenderAPI* m_Context = nullptr;
        std::atomic_uint32_t m_CurrentFrame = 0;
        uint32_t m_MaxFramesInFlight = 3;
        uint32_t m_ImageIndex = 0;

        std::vector<Ref<Fence>> m_InFlightFences;
        std::vector<Ref<Semaphore>> m_ImagesSemaphores;
        std::vector<Ref<Semaphore>> m_RenderSemaphores;
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
        static void BeginFrame();
        static void EndFrame();

        static void ClearRenderPass(ClearSettings settings = {});

        static void DrawIndexed(uint32_t count, const PrimitiveType& primitive = PrimitiveType::Triangles);
        static void DrawArrays(uint32_t vertexCount, const PrimitiveType& primitive = PrimitiveType::Lines);
        static void SetPipelineState(const Ref<PipelineState>& pipelineState);
        static void BeginRenderPass(const Ref<RenderPass>& renderPass);
        static void EndRenderPass();

        static Ref<RenderPass> GetDefaultRenderTarget();
        static Ref<PipelineState> GetDefaultPipeline();
        
        static void WaitAndRender();
    };

}

