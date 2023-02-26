#include "boidpch.h"

#include "Shader.h"


#include "Renderer.h"
#include "Engine/Engine/Application.h"
#include "Engine/Engine/Base.h"
#include "Platform/Opengl/GLShader.h"


namespace Polyboid
{
    Ref<Shader> Shader::Create(const ShaderType& shader, const std::string& shaderSoucePath)
    {

        auto renderApi = Application::Get().GetRenderAPI()->GetRenderAPIType();

        switch (renderApi)
        {
        case RenderAPIType::Opengl: return  std::make_shared<GLShader>(shader, shaderSoucePath);
        case RenderAPIType::Vulkan: 
        case RenderAPIType::Metal: 
        case RenderAPIType::Dx11: 
        case RenderAPIType::Dx12: 
            return nullptr;
        }

        return nullptr;
    }

    Ref<Shader> Shader::Create(const ShaderBinaryAndInfo& info)
    {
        auto& renderApi = Application::Get().GetRenderAPI();
        auto renderApiType = renderApi->GetRenderAPIType();

        switch (renderApiType)
        {
        case RenderAPIType::Opengl: return  std::make_shared<GLShader>(info);
        case RenderAPIType::Vulkan:
        case RenderAPIType::Metal:
        case RenderAPIType::Dx11:
        case RenderAPIType::Dx12:
            return nullptr;
        }

        return nullptr;
    }
}
