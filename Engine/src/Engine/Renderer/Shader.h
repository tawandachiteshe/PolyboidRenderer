#pragma once
#include <cstdint>
#include <string>

#include "RenderResource.h"
#include "Engine/Engine/Base.h"
#include "Engine/Engine/Shaders/ShaderCompiler.h"


namespace Polyboid
{
	enum class RenderResourceType;
	class Texture3D;
	class Texture2D;
	class SamplerState;
	class StorageBuffer;
	struct ShaderBinaryAndReflectionInfo;

	enum class ShaderType : uint8_t
    {
        Vertex,
        Fragment,
        Compute,
        None
    };

    enum class ShaderDataType
    {
        None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
    };

    static uint32_t ShaderDataTypeSize(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:    return 4;
        case ShaderDataType::Float2:   return 4 * 2;
        case ShaderDataType::Float3:   return 4 * 3;
        case ShaderDataType::Float4:   return 4 * 4;
        case ShaderDataType::Mat3:     return 4 * 3 * 3;
        case ShaderDataType::Mat4:     return 4 * 4 * 4;
        case ShaderDataType::Int:      return 4;
        case ShaderDataType::Int2:     return 4 * 2;
        case ShaderDataType::Int3:     return 4 * 3;
        case ShaderDataType::Int4:     return 4 * 4;
        case ShaderDataType::Bool:     return 1;
        default: return 0;
        }
        
        return 0;
    }



    struct ResourceBindingInfo
    {
        uint32_t Set = 0;
        uint32_t Binding = 0;
        RenderResourceType ResourceType = RenderResourceType::None;

    };

    using ShaderResourceRegistry = std::unordered_map<std::string, ResourceBindingInfo>;
    
    class Shader
    {
    
    public:
        //internals maybe
        virtual ~Shader() = default;
        
        virtual  void Bind() const = 0;
        virtual  void Unbind() const = 0;
        virtual ReflectionInfo GetShaderReflectionInfo() = 0;
        virtual ShaderType GetType() = 0;
        virtual ShaderResourceRegistry& GetShaderResourceType() = 0;

        static  Ref<Shader> Create(const ShaderType& shader, const std::string& shaderSourcePath);
        static  Ref<Shader> Create(const ShaderBinaryAndReflectionInfo& info);


    };


}
