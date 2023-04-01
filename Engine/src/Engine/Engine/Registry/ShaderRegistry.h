#pragma once
#include "Engine/Engine/Base.h"
#include "Engine/Engine/Shaders/ShaderCompiler.h"
#include "nlohmann/json.hpp"

namespace Polyboid
{
	class Shader;

	struct ShaderRegistryItem
	{
        std::string Checksum;
        std::string Path;
        std::string shaderPath;
	};

    class ShaderRegistry
    {
    private:
        static std::unordered_map<std::string, ShaderBinaryAndInfo> s_Registry;
        static  std::unordered_map<std::string, Ref<Shader>> s_RegistryShader;

    public:


        static void Init(RenderAPI* context);

        static bool Exist(const std::string& path);

        static Ref<Shader> Load(const std::string& path);

        Ref<Shader> Create(const std::string& path);




    };

}

