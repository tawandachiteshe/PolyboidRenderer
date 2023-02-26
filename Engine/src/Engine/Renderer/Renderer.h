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

namespace Polyboid
{
	class Material;
	class UniformBuffer;
	class RenderAPI;
	

	struct RendererStorage
    {
        Ref<RenderTarget> m_DefaultRenderTarget;
        Ref<PipelineState> m_DefaultPipelineState;
        RendererStorage() = default;
    };
	
    
    class Renderer
    {
    private:

        static Unique<RendererStorage> s_Data;
	 

    public:
        static void Init(const Ref<RenderAPI>& context);
        static void BeginDraw(const Ref<Camera>& camera);
        static void EndDraw();

        static void DrawIndexed(uint32_t count, const PrimitiveType& primitive = PrimitiveType::Triangles);
        static void DrawArrays(uint32_t vertexCount, const PrimitiveType& primitive = PrimitiveType::Lines);
        static void SetPipelineState(const Ref<PipelineState>& pipelineState);
        static void ClearDefaultRenderTarget(const ClearSettings& settings);
        static void BeginDefaultRenderPass();
        static void EndDefaultRenderPass();

        static Ref<RenderTarget> GetDefaultRenderTarget();
        static Ref<PipelineState> GetDefaultPipeline();
        
        static void WaitAndRender();
    };

}

