#include "boidpch.h"

#include "Shader.h"


#include "Renderer.h"
#include "Engine/Engine/Application.h"
#include "Engine/Engine/Base.h"


namespace Polyboid
{
    Ref<Shader> Shader::Create(const ShaderType& shader, const std::string& shaderSoucePath)
    {

        auto renderApi = Application::Get().GetRenderAPI()->GetRenderAPIType();

        switch (renderApi)
        {
        case RenderAPIType::Opengl:
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
        const auto renderApi = Application::Get().GetRenderAPI();
        const auto renderApiType = renderApi->GetRenderAPIType();

        switch (renderApiType)
        {
        case RenderAPIType::Opengl:
        case RenderAPIType::Vulkan:
        case RenderAPIType::Metal:
        case RenderAPIType::Dx11:
        case RenderAPIType::Dx12:
            return nullptr;
        }

        return nullptr;
    }
}
