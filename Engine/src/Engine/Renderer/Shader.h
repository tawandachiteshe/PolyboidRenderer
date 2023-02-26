#pragma once
#include <cstdint>
#include <memory>
#include <string>

#include "UniformBuffer.h"
#include "Engine/Engine/Base.h"
#include "glm/fwd.hpp"


namespace Polyboid
{
	class Texture3D;
	class Texture;
	class SamplerState;
	class ShaderStorageBuffer;
	struct ShaderBinaryAndInfo;

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
        }
        
        return 0;
    }

    
    class Shader
    {
    
    public:
        //internals maybe
        virtual ~Shader() = default;
        
        virtual  void Bind() const = 0;
        virtual  void Unbind() const = 0;
        static  Ref<Shader> Create(const ShaderType& shader, const std::string& shaderSourcePath);
        static  Ref<Shader> Create(const ShaderBinaryAndInfo& info);

        virtual void SetUniformBuffer(const std::string& name, const Ref<UniformBuffer>& value) = 0;
        virtual void SetShaderStorageBuffer(const std::string& name, const Ref<ShaderStorageBuffer>& value) = 0;
        virtual void SetSampler(const std::string& name, const Ref<SamplerState>& value) = 0;
        virtual void SetImage(const std::string& name, const Ref<SamplerState>& value) = 0;
        virtual void SetTexture2D(const std::string& name, const Ref<Texture>& value) = 0;
        virtual void SetTexture3D(const std::string& name, const Ref<Texture3D>& value) = 0;

        [[nodiscard]] virtual Ref<UniformBuffer> GetUniformBuffer(const std::string& name) const = 0;
        [[nodiscard]] virtual Ref<ShaderStorageBuffer> GetShaderStorageBuffer(const std::string& name) const = 0;
        [[nodiscard]] virtual Ref<SamplerState> GetSampler(const std::string& name) const = 0;
        [[nodiscard]] virtual Ref<SamplerState> GetImage(const std::string& name) const = 0;
        [[nodiscard]] virtual ShaderType GetType() = 0;

  
    
    };


}
