﻿#include "boidpch.h"

#include "Renderer.h"

#include <spdlog/spdlog.h>

#include "Material.h"
#include "RenderAPI.h"
#include "RenderCommand.h"
#include "Renderer2D.h"
#include "Texture2D.h"
#include "UniformBuffer.h"
#include "Engine/Engine/AssetManager.h"
#include "glad/glad.h"
#include "glm/vec4.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace Polyboid
{

    Unique<RendererStorage> Renderer::s_RenderStorage = std::make_unique<RendererStorage>();
    Unique<RenderAPI> Renderer::s_RenderApi = std::make_unique<RenderAPI>();
    
    void Renderer::Init() {

        POLYBOID_PROFILE_FUNCTION();

        RenderAPI::Init();
        Renderer2D::Init();
        s_RenderStorage->m_UB = UniformBuffer::MakeUniformBuffer(sizeof(glm::mat4) * 2);
    }

    void Renderer::Clear(const ClearMode& mode)
    {
        RenderAPI::Clear(mode);
    }

    void Renderer::SetClearColor(const glm::vec4& color)
    {
        RenderAPI::SetClearColor(color);
    }


    void Renderer::Submit(const Ref<VertexBufferArray>& va, const Ref<Shader>& shader, const glm::mat4& transform)
    {
        POLYBOID_PROFILE_FUNCTION();

        if (va != nullptr)
        {
            shader->Bind();
            va->Bind();
            shader->SetMat4("uTransform", transform);

            if (va->GetIndicesCount() > 0)
            {
                RenderCommand::DrawIndexed(va->GetIndicesCount());
            }
            else
            {
                //RenderCommand::DrawIndexed(va->GetIndexBuffer()->GetCount());
            }

           
        }

    }

    void Renderer::Submit(const std::vector<Ref<VertexBufferArray>>& vas, const Ref<Shader>& shader,
	    const glm::mat4& transform)
    {
	    for (auto& va : vas)
	    {
            Submit(va, shader, transform);
	    }
    }

    void Renderer::Submit(const std::pair<Ref<VertexBufferArray>, Ref<Material>>& va, const Ref<Shader>& shader,
	    const glm::mat4& transform)
    {

        shader->Bind();
        auto& mat = va.second;
        shader->SetFloat3("uMaterial.Albedo", mat->GetAlbedo());
        shader->SetFloat3("uMaterial.AO", mat->GetAO());
       
        AssetManager::GetTexture(mat->mDiffuseTexture)->Bind(4);
        AssetManager::GetTexture(mat->mNormalsTexture)->Bind(5);
        AssetManager::GetTexture(mat->mMetallicTexture)->Bind(6);
        AssetManager::GetTexture(mat->mAOTexture)->Bind(7);
        AssetManager::GetTexture(mat->mRoughnessTexture)->Bind(8);

        //float specularExponent = glm::exp2(mat->GetRoughness() * 11) + 2;
        shader->SetFloat("uMaterial.Roughness", 1 - mat->GetRoughness());
        shader->SetFloat("uMaterial.Metallic", mat->GetMetallic());

        shader->SetInt("uMaterial.Textures.albedo", 4);
        shader->SetInt("uMaterial.Textures.normals", 5);
        shader->SetInt("uMaterial.Textures.metallic", 6);
        shader->SetInt("uMaterial.Textures.ao", 7);
        shader->SetInt("uMaterial.Textures.roughness", 8);

        Submit(va.first, shader, transform);

    }

    void Renderer::Submit(const std::vector<std::pair<Ref<VertexBufferArray>, Ref<Material>>>& vas,
	    const Ref<Shader>& shader, const glm::mat4& transform)
    {
        for (auto& va : vas)
        {
            Submit(va, shader, transform);
        }
    }

    void Renderer::BeginDraw(const Ref<Camera>& camera)
    {
        POLYBOID_PROFILE_FUNCTION();

        s_RenderStorage->m_UB->SetData(glm::value_ptr(camera->GetProjection()), sizeof(glm::mat4));
        s_RenderStorage->m_UB->SetData(glm::value_ptr(camera->GetView()), sizeof(glm::mat4), 64);

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

    void Renderer::CullMode(const Polyboid::CullMode& mode)
    {
	    switch (mode)
	    {
	    case CullMode::Front:
            RenderAPI::CullFront();
            break;
	    case CullMode::Back:
            RenderAPI::CullBack();
            break;
	    case CullMode::FrontBack:
            RenderAPI::CullFrontBack();
            break;
	    case CullMode::None:
            RenderAPI::CullNone();
            break;
	    }
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
