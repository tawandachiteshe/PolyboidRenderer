#pragma once
#include "Engine/Engine/Shaders/ShaderCompiler.h"
#include "Engine/Renderer/Shader.h"


namespace Polyboid
{
	class GLTexture3D;
	class GLTexture2D;
	class GLShaderStorageBuffer;
	class GLUniformBuffer;

	class GLShader : public Shader
    {
        enum class ShaderDataType
        {
            //We Support only these for now
            Int,
            IntArray,
            Float,
            Float2,
            Float3,
            Float4,
            Mat3,
            Mat4
        };

        struct UniformInfo
        {
            ShaderDataType DataType;
            uint32_t DataSize;

            //Used for Arrays only
            uint32_t Count;
        };
        
    private:

        void CheckProgramError();
        
    private:
        
        ShaderType m_ShaderType;
        uint32_t m_ShaderProgram;
        ShaderBinaryAndInfo m_Info;
        std::map<std::string, Ref<GLUniformBuffer>> m_UniformBuffers;
        std::map<std::string, Ref<GLShaderStorageBuffer>> m_SSOBBuffers;
        std::map<std::string, Ref<SamplerState>> m_Samplers;
        std::map<std::string, Ref<GLTexture2D>> m_Textures2D;
        std::map<std::string, Ref<GLTexture3D>> m_Textures3D;
        std::map<std::string, Ref<SamplerState>> m_Images;

        
    public:

        using UniformValuesMap = std::unordered_map<std::string, UniformInfo>;

        GLShader(const ShaderType& shaderType, const std::string& shaderSource);
        GLShader(const ShaderBinaryAndInfo& shaderBinaryInfo);
        ~GLShader() override;
        

        void Bind() const override;
        void Unbind() const override;
        void SetUniformBuffer(const std::string& name, const Ref<UniformBuffer>& value) override;
        void SetShaderStorageBuffer(const std::string& name, const Ref<ShaderStorageBuffer>& value) override;
        void SetSampler(const std::string& name, const Ref<SamplerState>& value) override;
        void SetImage(const std::string& name, const Ref<SamplerState>& value) override;

        void SetTexture2D(const std::string& name, const Ref<Texture>& value) override;
        void SetTexture3D(const std::string& name, const Ref<Texture3D>& value) override;
        [[nodiscard]] ShaderType GetType() override;


        [[nodiscard]] Ref<UniformBuffer> GetUniformBuffer(const std::string& name) const override;
        [[nodiscard]] Ref<ShaderStorageBuffer> GetShaderStorageBuffer(const std::string& name) const override;
        [[nodiscard]] Ref<SamplerState> GetSampler(const std::string& name) const override;
        [[nodiscard]] Ref<SamplerState> GetImage(const std::string& name) const override;

        friend class GLPipelineState;
    };

  
}
