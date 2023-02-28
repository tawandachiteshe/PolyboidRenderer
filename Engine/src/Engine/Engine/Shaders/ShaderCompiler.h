#pragma once

#include "shaderc/shaderc.hpp"

namespace Polyboid
{

	struct ShaderImageInfo
	{

        //TODO: Add format converter SHaderImageInfoType thing;

        uint32_t Binding = 0;
        uint32_t Set = 0;
        std::string Name;


        //Vulkan only



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

        using ShaderBinaryMap = std::unordered_map<std::string, ShaderBinaryAndInfo>;

        static ShaderBinaryAndInfo Compile(const std::filesystem::path& path, const std::string& rootPath);
        static void Reflect(const std::string& shaderReflectJson, ReflectionInfo& info);
        static ShaderBinaryMap CompileShadersFromPath(const std::filesystem::path& directoryPath);
        static bool Dump(const ShaderBinaryMap& shaderBinaries, const std::filesystem::path& cachePath);
        static ShaderBinaryMap LoadFromDump(const std::filesystem::path& cachePath);
        static std::string SpirvCheckSum(const std::vector<uint32_t>& spirv);
    
    };

}

