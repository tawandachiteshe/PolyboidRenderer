#pragma once

#include "ShadercIncluder.h"
#include "Engine/Renderer/RenderAPI.h"
#include "shaderc/shaderc.hpp"

namespace Polyboid
{
	class RenderAPI;

	struct ShaderImageInfo
	{

        //TODO: Add format converter SHaderImageInfoType thing;

        uint32_t Binding = 0;
        uint32_t Set = 0;
        std::string Name;


        //Vulkan only



	};

	struct PushConstantInfo
    {
        std::string Name;

    };

	struct ShaderStorageInfo
	{
        uint32_t Binding = 0;
        uint32_t Set = 0;
        std::string Name;
        bool isReadOnly = false;
        uint32_t blockSize = 0;

	};

	struct UniformBufferInfo
	{
        uint32_t Binding = 0;
        uint32_t Set = 0;
        std::string Name;
        uint32_t blockSize = 0;

	};

	// struct ReflectionInfo
	// {
	// 	
	// };

    struct ReflectionInfo
    {
        std::map<std::string, ShaderImageInfo> images;
        std::map<std::string, ShaderImageInfo> textures;
        std::map<std::string, ShaderStorageInfo> ssbos;
        std::map<std::string, UniformBufferInfo> ubos;
        std::map<std::string, PushConstantInfo> pushConstants;

	};

	struct ShaderBinaryAndInfo
	{
        
        uint8_t type;
        std::string filePath;
        std::string shaderReflect;
        std::string spirvChecksum;
        std::vector<uint32_t> Spirv;
        ReflectionInfo reflectionInfo;

	};

    class ShaderCompiler
    {
    public:
	    struct ShaderCompilerData
	    {

	    private:

            void SetupVulkan()
            {
                m_Options.SetTargetEnvironment(shaderc_target_env_vulkan, 0);

            }

            void SetupOpenGL()
            {
                m_Options.SetTargetEnvironment(shaderc_target_env::shaderc_target_env_opengl, 0);
            }

	    public:
            shaderc::Compiler m_Compiler;
            shaderc::CompileOptions m_Options;
            const RenderAPI* m_Context = nullptr;
            std::string m_IncludePath;
            bool m_Debug = true;

            ShaderCompilerData(const RenderAPI* context, const std::string& includePath): m_Context(context), m_IncludePath(includePath)
            {
                const auto renderAPI = context->GetRenderAPIType();
                switch (renderAPI)
                {
                case RenderAPIType::Opengl: SetupOpenGL(); break;
                case RenderAPIType::Vulkan: SetupVulkan();  break;
                case RenderAPIType::Metal: break;
                case RenderAPIType::Dx11: break;
                case RenderAPIType::Dx12: break;
                }

                //TODO: make this in more robust
                m_Options.SetOptimizationLevel(shaderc_optimization_level::shaderc_optimization_level_zero);
                //only for vulkan
                //options.SetTargetSpirv(shaderc_targe)
                m_Options.SetGenerateDebugInfo();

                std::unique_ptr<shaderc::CompileOptions::IncluderInterface> includer = std::make_unique<
                    ShadercIncluder>(includePath);
                m_Options.SetIncluder(std::move(includer));



            }

	    };

        static Ref<ShaderCompilerData> s_Data;

    public:


        using ShaderBinaryMap = std::unordered_map<std::string, ShaderBinaryAndInfo>;
        static void Init(const RenderAPI* context, const std::string& includePath);

        static ShaderBinaryAndInfo Compile(const std::filesystem::path& path, const std::string& rootPath);
        static void Reflect(const std::string& shaderReflectJson, ReflectionInfo& info);
        static ShaderBinaryMap CompileShadersFromPath(const std::filesystem::path& directoryPath);
        static bool Dump(const ShaderBinaryMap& shaderBinaries, const std::filesystem::path& cachePath);
        static ShaderBinaryMap LoadFromDump(const std::filesystem::path& cachePath);
        static std::string SpirvCheckSum(const std::vector<uint32_t>& spirv);
    
    };

}

