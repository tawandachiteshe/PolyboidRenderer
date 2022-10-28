#include "Renderer.h"

#include <spdlog/spdlog.h>

#include "RenderAPI.h"
#include "Renderer2D.h"
#include "glad/glad.h"
#include "glm/vec4.hpp"

namespace Polyboid
{

    Unique<RendererStorage> Renderer::s_RenderStorage = std::make_unique<RendererStorage>();
    Unique<RenderAPI> Renderer::s_RenderApi = std::make_unique<RenderAPI>();
    
    void Renderer::Init() {

        RenderAPI::Init();
        Renderer2D::Init();
    }

    void Renderer::Clear(const glm::vec4& color )
    {
        RenderAPI::Clear(color);
    }

    void Renderer::Submit(const Ref<VertexBufferArray>& va, const Ref<Shader>& shader, const glm::mat4& transform)
    {
        shader->Bind();
        s_RenderStorage->m_VA = va;
        s_RenderStorage->m_Shader = shader;
        s_RenderStorage->m_Shader->SetMat4("uTransform", transform);
    }

    void Renderer::BeginDraw(const Ref<Camera3D>& camera)
    {
         s_RenderStorage->m_Shader->SetMat4("uViewProj", camera->GetViewProjectionMatrix());
    }

    void Renderer::EndDraw()
    {
    }


    void Renderer::DrawIndexed(uint32_t _count, uint32_t elementCount)
    {
        s_RenderStorage->m_Shader->Bind();
        s_RenderStorage->m_VA->Bind();

       
        const auto count = s_RenderStorage->m_VA->GetIndexBuffer()->GetCount();
        const auto countIf = static_cast<GLsizei>(_count == 0 ? count : _count);

        RenderAPI::DrawIndexed(countIf, 2);
       
    }

    void Renderer::CreateViewPort(const glm::vec2& viewportSize)
    {
        RenderAPI::CreateViewport(viewportSize);
    }

    
    
}
