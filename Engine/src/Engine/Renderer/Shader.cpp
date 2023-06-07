#include "boidpch.h"

#include "Shader.h"


#include "RenderCommand.h"
#include "Engine/Engine/Application.h"
#include "Engine/Engine/Base.h"


namespace Polyboid
{

    Ref<Shader> Shader::Create(const ShaderBinaryAndReflectionInfo& info)
    {
        const auto renderApi = RenderAPI::Get();

        return renderApi->CreateShader(info);
    }
}
