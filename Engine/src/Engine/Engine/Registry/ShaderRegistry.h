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

 
    using GraphicsShaders = std::pair<Ref<Shader>, Ref<Shader>>;

    class ShaderRegistry
    {
    private:
        static std::unordered_map<std::string, ShaderBinaryAndReflectionInfo> s_Registry;
        static  std::unordered_map<std::string, Ref<Shader>> s_RegistryShader;
        static std::string s_Path;

    public:


        static void Init(RenderAPI* context);

        static bool Exist(const std::string& path);
        static void SetPath(const std::string& path);

        static Ref<Shader> Load(const std::string& path);
        static GraphicsShaders LoadGraphicsShaders(const std::string& vtxPath, const std::string& fragPath);
        static GraphicsShaders LoadGraphicsShaders(const std::string& path);

        Ref<Shader> Create(const std::string& path);




    };

}

