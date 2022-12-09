#pragma once
#include "Camera3D.h"
#include "Shader.h"
#include "VertexBufferArray.h"
#include "Engine/Engine/Base.h"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "glm/matrix.hpp"

namespace Polyboid
{
	class RenderAPI;

	struct RendererStorage
    {
        Ref<VertexBufferArray> m_VA = nullptr;
        Ref<Shader> m_Shader = nullptr;

        RendererStorage() = default;
    };
    
    class Renderer
    {
    private:
        static Unique<RendererStorage> s_RenderStorage;
        static Unique<RenderAPI> s_RenderApi;
    public:
        static void Init();
        static void Clear(const glm::vec4& color = { 0.2, 0.2, 0.2, 1.0 });
        static void Submit(const Ref<VertexBufferArray>& va, const Ref<Shader>& shader, const glm::mat4& transform = glm::mat4(1.0f));

        static void BeginDraw();
        static void EndDraw();
        static void EnableDepthMask();
        static void DisableDepthMask();

        static void DrawIndexed(uint32_t count = 0, uint32_t elementCount = 4);
        static void CreateViewPort(const glm::vec2& viewportSize);
    };

}

