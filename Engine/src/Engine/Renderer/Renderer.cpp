#include "Renderer.h"

#include <spdlog/spdlog.h>

#include "glad/glad.h"
#include "glm/vec4.hpp"

namespace Polyboid
{

    Unique<RendererStorage> Renderer::s_RenderStorage = std::make_unique<RendererStorage>();
    
    void Renderer::Init()
    {

    }

    void Renderer::Clear(const glm::vec4& color )
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(color.x, color.y, color.z, color.w);
    }

    void Renderer::Submit(const Ref<VertexBufferArray>& va, const Ref<Shader>& shader)
    {
        s_RenderStorage->m_VA = va;
        s_RenderStorage->m_Shader = shader;
    }

    void Renderer::Draw()
    {
        s_RenderStorage->m_Shader->Bind();
        s_RenderStorage->m_VA->Bind();
        auto count = s_RenderStorage->m_VA->GetIndexBuffer()->GetCount();

        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    }

    void Renderer::CreateViewPort(const glm::vec2& viewportSize)
    {
        glViewport(0, 0, viewportSize.x, viewportSize.y);
    }

    
    
}
