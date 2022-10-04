#include "Renderer.h"

#include <spdlog/spdlog.h>

#include "Engine/Core/Application.h"
#include "glad/glad.h"
#include "glm/vec4.hpp"

namespace Polyboid
{

    Unique<RendererStorage> Renderer::s_RenderStorage = std::make_unique<RendererStorage>();
    
    void Renderer::Init() {

        
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glCullFace(GL_BACK);
        glFrontFace(GL_CW);
    
        
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

    void Renderer::BeginDraw(const Ref<Camera3D>& camera)
    {
         s_RenderStorage->m_Shader->SetMat4("uViewProj", camera->GetViewProjectionMatrix());
    }

    void Renderer::EndDraw()
    {
    }


    void Renderer::Draw(const glm::mat4& transform)
    {
        s_RenderStorage->m_Shader->Bind();
        s_RenderStorage->m_VA->Bind();

        s_RenderStorage->m_Shader->SetMat4("uTransform", transform);
        const auto count = s_RenderStorage->m_VA->GetIndexBuffer()->GetCount();

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(count), GL_UNSIGNED_SHORT, nullptr);
    }

    void Renderer::CreateViewPort(const glm::vec2& viewportSize)
    {
        glViewport(0, 0, static_cast<GLsizei>(viewportSize.x), static_cast<GLsizei>(viewportSize.y));
    }

    
    
}
