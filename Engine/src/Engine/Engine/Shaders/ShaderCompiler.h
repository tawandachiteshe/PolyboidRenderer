#pragma once

#include "Engine/Engine/Base.h"
#include "shaderc/shaderc.hpp"
#include <filesystem>
#include <map>
#include <unordered_map>

namespace Polyboid
{
	class RenderAPI;

	enum class ShaderImageType
	{
		SamplerBuffer,
        ImageBuffer,
        SamplerImage
	};

	enum class ShaderTextureType
	{
		Texture2D,
        Texture3D //These area sampler cubes technically
	};

	inline ShaderImageType GlslToShaderImageType(const std::string& type)
    {
	    if (type == "image2D")
	    {
            return ShaderImageType::SamplerImage;
	    }

		if(type == "imageBuffer")
	    {
		    return ShaderImageType::ImageBuffer;
	    }

		if (type == "samplerBuffer")
	    {
		    return ShaderImageType::SamplerBuffer;
	    }

	    return ShaderImageType::SamplerImage;
    }

    inline ShaderTextureType GlslToShaderTextureType(const std::string& type)
    {
        if (type == "sampler2D")
        {
            return ShaderTextureType::Texture2D;
        }

		if (type == "samplerCube")
        {
	        return ShaderTextureType::Texture3D;
        }

        return ShaderTextureType::Texture2D;
    }



	struct ShaderImageInfo
	{

        //TODO: Add format converter SHaderImageInfoType thing;

        uint32_t Binding = 0;
        uint32_t Set = 0;
        uint32_t arrayLength = 1;
        std::string Name;
        ShaderImageType imageType;

        //Vulkan only



	};


    struct ShaderTextureInfo
    {

        //TODO: Add format converter SHaderImageInfoType thing;

        uint32_t Binding = 0;
        uint32_t Set = 0;
        uint32_t arrayLength = 1;
        std::string Name;
        ShaderTextureType textureType;

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
        std::map<std::string, ShaderTextureInfo> textures;
        std::map<std::string, ShaderStorageInfo> ssbos;
        std::map<std::string, UniformBufferInfo> ubos;
        std::map<std::string, PushConstantInfo> pushConstants;

	};

	struct ShaderBinaryAndReflectionInfo
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
                m_Options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);

            }

            void SetupOpenGL()
            {
                __debugbreak();
                m_Options.SetTargetEnvironment(shaderc_target_env::shaderc_target_env_opengl, 0);
            }

	    public:
            shaderc::Compiler m_Compiler;
            shaderc::CompileOptions m_Options;
            const RenderAPI* m_Context = nullptr;
            std::string m_IncludePath;
            bool m_Debug = true;

            ShaderCompilerData(const RenderAPI* context, const std::string& includePath);
	    };

        static Ref<ShaderCompilerData> s_Data;

    public:


        using ShaderBinaryMap = std::unordered_map<std::string, ShaderBinaryAndReflectionInfo>;
        static void Init(const RenderAPI* context, const std::string& includePath);

        static ShaderBinaryAndReflectionInfo Compile(const std::filesystem::path& path, const std::string& rootPath);
        static void Reflect(const std::string& shaderReflectJson, ReflectionInfo& info);
        static ShaderBinaryMap CompileShadersFromPath(const std::filesystem::path& directoryPath);
        static bool Dump(const ShaderBinaryMap& shaderBinaries, const std::filesystem::path& cachePath);
        static ShaderBinaryMap LoadFromDump(const std::filesystem::path& cachePath);
        static std::string SpirvCheckSum(const std::vector<uint32_t>& spirv);
    
    };

}

