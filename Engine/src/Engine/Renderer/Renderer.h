#pragma once
#include "Shader.h"
#include "VertexBufferArray.h"
#include "Engine/Core/Base.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"


namespace Polyboid
{
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
    public:
        static void Init();
        static void Clear(const glm::vec4& color = { 0.2, 0.2, 0.2, 1.0 });
        static void Submit(const Ref<VertexBufferArray>& va, const Ref<Shader>& shader);
        static void Draw();
        static void CreateViewPort(const glm::vec2& viewportSize);
    };

}

