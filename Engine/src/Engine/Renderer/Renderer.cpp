#include "boidpch.h"

#include "Renderer.h"

#include <spdlog/spdlog.h>

#include "RenderAPI.h"
#include "RenderCommand.h"
#include "Renderer2D.h"
#include "glad/glad.h"
#include "glm/vec4.hpp"

namespace Polyboid
{

    Unique<RendererStorage> Renderer::s_RenderStorage = std::make_unique<RendererStorage>();
    Unique<RenderAPI> Renderer::s_RenderApi = std::make_unique<RenderAPI>();
    
    void Renderer::Init() {

        POLYBOID_PROFILE_FUNCTION();

        RenderAPI::Init();
        Renderer2D::Init();
    }

    void Renderer::Clear(const glm::vec4& color )
    {
        POLYBOID_PROFILE_FUNCTION();
       
        RenderAPI::Clear(color);
    }

    void Renderer::Submit(const Ref<VertexBufferArray>& va, const Ref<Shader>& shader, const glm::mat4& transform)
    {
        POLYBOID_PROFILE_FUNCTION();

        shader->Bind();
        va->Bind();
        shader->SetMat4("uTransform", transform);
        RenderCommand::DrawIndexed(va->GetIndexBuffer()->GetCount());
    }

    void Renderer::BeginDraw()
    {
        POLYBOID_PROFILE_FUNCTION();

    }

    void Renderer::EndDraw()
    {
        POLYBOID_PROFILE_FUNCTION();
    }

    void Renderer::EnableDepthMask()
    {
        glDepthMask(GL_TRUE);
    }

    void Renderer::DisableDepthMask()
    {
        glDepthMask(GL_FALSE);
    }


    void Renderer::DrawIndexed(uint32_t _count, uint32_t elementCount)
    {
        POLYBOID_PROFILE_FUNCTION();
       
        s_RenderStorage->m_Shader->Bind();
        s_RenderStorage->m_VA->Bind();

       
        const auto count = s_RenderStorage->m_VA->GetIndexBuffer()->GetCount();
        const auto countIf = static_cast<GLsizei>(_count == 0 ? count : _count);

        RenderAPI::DrawIndexed(countIf, 2);
       
    }

    void Renderer::CreateViewPort(const glm::vec2& viewportSize)
    {
        POLYBOID_PROFILE_FUNCTION();

        RenderAPI::CreateViewport(viewportSize);
    }

    
    
}
