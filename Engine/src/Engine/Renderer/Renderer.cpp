#include "boidpch.h"

#include "Renderer.h"

#include <spdlog/spdlog.h>

#include "Material.h"
#include "RenderAPI.h"
#include "RenderCommand.h"
#include "Renderer2D.h"
#include "ShaderBufferStorage.h"
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
        s_RenderStorage->m_CameraDataUB = UniformBuffer::MakeUniformBuffer(sizeof(CameraData));
        s_RenderStorage->m_MaterialStorage = ShaderBufferStorage::Make(sizeof(MaterialData) * 128);
    }

    void Renderer::Clear(const ClearMode& mode)
    {
        RenderAPI::Clear(mode);
    }

    void Renderer::SetClearColor(const glm::vec4& color)
    {
        POLYBOID_PROFILE_FUNCTION();
        RenderAPI::SetClearColor(color);
    }


    void Renderer::Submit(const Ref<VertexBufferArray>& va, const Ref<Shader>& shader, const glm::mat4& transform )
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


    void Renderer::Submit(const MeshDataRenderer& meshData, const Ref<Shader>& shader,
	    const glm::mat4& transform)
    {
        POLYBOID_PROFILE_FUNCTION();
        shader->Bind();

        uint32_t materialOffset = 0;
        int32_t materialIndex = 0;
        shader->Bind();
    	for (auto& materialVA : meshData)
        {

        	auto& [material, vertexBufferArray] = materialVA;
            AssetManager::GetTexture(material->mDiffuseTexture)->Bind(0);
            AssetManager::GetTexture(material->mNormalsTexture)->Bind(1);
            AssetManager::GetTexture(material->mMetallicTexture)->Bind(2);

            shader->SetInt("uMaterialIndex", 0);

            s_RenderStorage->m_MaterialStorage->Bind(1);
            s_RenderStorage->m_MaterialStorage->SetData(&material->GetData(), sizeof(MaterialData), materialOffset);
            Submit(vertexBufferArray, shader, transform);

            materialOffset += sizeof(MaterialData);
            materialIndex++;
        }

    }


    void Renderer::BeginDraw(const Ref<Camera>& camera)
    {
        POLYBOID_PROFILE_FUNCTION();

        CameraData data = {};
        data.Projection = camera->GetProjection();
        data.View = camera->GetView();
    	data.CameraPosition = camera->GetPosition();


        s_RenderStorage->m_CameraDataUB->SetData(&data, sizeof(data));

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

        RenderAPI::DrawIndexed(countIf, 4);
       
    }

    void Renderer::CreateViewPort(const glm::vec2& viewportSize)
    {
        POLYBOID_PROFILE_FUNCTION();

        RenderAPI::CreateViewport(viewportSize);
    }

    
    
}
