#pragma once

#include "shaderc/shaderc.hpp"
#include <pods/pods.h>

namespace Polyboid
{

	struct ShaderImageInfo
	{

        //TODO: Add format converter SHaderImageInfoType thing;

        uint32_t Binding = 0;
        uint32_t Set = 0;
        std::string Name;

        PODS_SERIALIZABLE(
            PODS_MDR(Binding),
            PODS_MDR(Set),
            PODS_MDR(Name))


        //Vulkan only



	};

	struct ShaderStorageInfo
	{
        uint32_t Binding = 0;
        uint32_t Set = 0;
        std::string Name;
        bool isReadOnly = false;
        uint32_t blockSize = 0;

        PODS_SERIALIZABLE(
            PODS_MDR(Binding),
            PODS_MDR(Set),
            PODS_MDR(Name),
            PODS_MDR(isReadOnly),
            PODS_MDR(blockSize)
        )

	};

	struct UniformBufferInfo
	{
        uint32_t Binding = 0;
        uint32_t Set = 0;
        std::string Name;
        uint32_t blockSize = 0;

        PODS_SERIALIZABLE(
            PODS_MDR(Binding),
            PODS_MDR(Set),
            PODS_MDR(Name),
            PODS_MDR(blockSize)
        )
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

        PODS_SERIALIZABLE(
            PODS_MDR(images),
            PODS_MDR(textures),
            PODS_MDR(ssbos),
            PODS_MDR(ubos)
        )

	};

	struct ShaderBinaryAndInfo
	{
        
        uint8_t type;
        std::string filePath;
        std::string shaderReflect;
        std::string spirvChecksum;
        std::vector<uint32_t> Spirv;
        ReflectionInfo reflectionInfo;


        PODS_SERIALIZABLE(
            PODS_MDR(Spirv),
            PODS_MDR(type),
            PODS_MDR(shaderReflect),
            PODS_MDR(spirvChecksum),
            PODS_MDR(reflectionInfo),
            PODS_MDR(filePath)
        )
	};

    class ShaderCompiler
    {
    public:

        using ShaderBinaryMap = std::unordered_map<std::string, ShaderBinaryAndInfo>;

        static ShaderBinaryAndInfo Compile(const std::filesystem::path& path, const std::string& rootPath);
        static void Reflect(const std::string& shaderReflectJson, ReflectionInfo& info);
        static ShaderBinaryMap CompileShadersFromPath(const std::filesystem::path& directoryPath);
        static bool Dump(const ShaderBinaryMap& shaderBinaries, const std::filesystem::path& cachePath);
        static ShaderBinaryMap LoadFromDump(const std::filesystem::path& cachePath);
        static std::string SpirvCheckSum(const std::vector<uint32_t>& spirv);
    
    };

}

