#include "boidpch.h"
#include "GLShader.h"

#include <spdlog/spdlog.h>

#include "GLSamplerState.h"
#include "Engine/Engine/Shaders/ShaderCompiler.h"
#include "glad/glad.h"

namespace Polyboid
{
	class GLTexture2D;

	void GLShader::CheckProgramError()
    {
        GLint isLinked = 0;
        glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, (int *)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(m_ShaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(m_ShaderProgram, maxLength, &maxLength, &infoLog[0]);

            std::string shaderTypeName;

            switch (m_ShaderType)
            {
                case ShaderType::Vertex: shaderTypeName = "Vertex"; break;
                case ShaderType::Fragment: shaderTypeName = "Fragment"; break;
                case ShaderType::Compute: shaderTypeName = "Compute"; break;
            }
            
            spdlog::error("{} shader Error: {}", shaderTypeName, infoLog.data());
	
            // We don't need the program anymore.
            glDeleteProgram(m_ShaderProgram);
        }
    }


    GLShader::GLShader(const ShaderType& shaderType, const std::string& shaderSource): m_ShaderType(shaderType)
    {

        const char* shaderSrc = shaderSource.c_str();
        switch (shaderType)
        {
            case ShaderType::Vertex:
                m_ShaderProgram = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &shaderSrc);
                break;
            case ShaderType::Fragment:
                m_ShaderProgram = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &shaderSrc);
                break;
            case ShaderType::Compute:
                m_ShaderProgram = glCreateShaderProgramv(GL_COMPUTE_SHADER, 1, &shaderSrc);
                break;
        }

        CheckProgramError();
        
    }

    GLShader::GLShader(const ShaderBinaryAndInfo& shaderBinaryInfo): m_Info(shaderBinaryInfo)
    {

        
        uint32_t shader = 0;

        m_ShaderType = static_cast<ShaderType>(shaderBinaryInfo.type);

        switch (m_ShaderType)
        {
	        case ShaderType::Vertex: shader = glCreateShader(GL_VERTEX_SHADER); break;
	        case ShaderType::Fragment: shader = glCreateShader(GL_FRAGMENT_SHADER); break;
	        case ShaderType::Compute: shader = glCreateShader(GL_COMPUTE_SHADER); break;
            case ShaderType::None: shader = 0; break;
        }

        glShaderBinary(1, &shader, GL_SHADER_BINARY_FORMAT_SPIR_V, shaderBinaryInfo.Spirv.data(), shaderBinaryInfo.Spirv.size() * sizeof(uint32_t));

        //TODO: bring entry point here
    	glSpecializeShader(shader, "main", 0, nullptr, nullptr);

        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

            // We don't need the shader anymore.
            glDeleteShader(shader);

            infoLog.push_back(0);
            // Use the infoLog as you see fit.

            // In this simple program , we'll just leave

            spdlog::error("Shader compilation failed: {}", infoLog.data());

            return;
        }

        m_ShaderProgram = glCreateProgram();
        glProgramParameteri(m_ShaderProgram, GL_PROGRAM_SEPARABLE, GL_TRUE);

        // Attach our shaders to our program
        glAttachShader(m_ShaderProgram, shader);

        // Link our program
        glLinkProgram(m_ShaderProgram);

        CheckProgramError();

        glDetachShader(m_ShaderProgram, shader);

    }

    GLShader::~GLShader()
    {
        glDeleteProgram(m_ShaderProgram);
    }

    void GLShader::Bind() const
    {
        glUseProgram(m_ShaderProgram);
    }

    void GLShader::Unbind() const
    {
        glUseProgram(0);
    }

    void GLShader::SetUniformBuffer(const std::string& name, const Ref<UniformBuffer>& value)
    {
        auto& ubos = m_Info.reflectionInfo.ubos;

        if (ubos.find(name) != ubos.end())
        {
            m_UniformBuffers[name] = std::reinterpret_pointer_cast<GLUniformBuffer>(value);
        }

    }

    void GLShader::SetShaderStorageBuffer(const std::string& name, const Ref<ShaderStorageBuffer>& value)
    {
        auto& ssbos = m_Info.reflectionInfo.ssbos;

        if (ssbos.find(name) != ssbos.end())
        {
            m_SSOBBuffers[name] = std::reinterpret_pointer_cast<GLShaderStorageBuffer>(value);
        }

    }

    void GLShader::SetSampler(const std::string& name, const Ref<SamplerState>& value)
    {
        auto& textures = m_Info.reflectionInfo.textures;

        if (textures.find(name) != textures.end())
        {
            m_Samplers[name] = std::reinterpret_pointer_cast<GLSamplerState>(value);
        }
    }

    void GLShader::SetImage(const std::string& name, const Ref<SamplerState>& value)
    {
        auto& images = m_Info.reflectionInfo.images;

        if (images.find(name) != images.end())
        {
            m_Samplers[name] = std::reinterpret_pointer_cast<GLSamplerState>(value);
        }
    }

    void GLShader::SetTexture2D(const std::string& name, const Ref<Texture>& value)
    {
        auto it = m_Textures2D.find(name);
        if (it != m_Textures2D.end())
        {
            m_Textures2D[name] = std::reinterpret_pointer_cast<GLTexture2D>(value);;
        }

    }

    void GLShader::SetTexture3D(const std::string& name, const Ref<Texture3D>& value)
    {




        auto it = m_Textures3D.find(name);
        if (it != m_Textures3D.end())
        {
            m_Textures3D[name] = std::reinterpret_pointer_cast<GLTexture3D>(value);;
        }

    }

    ShaderType GLShader::GetType()
    {
        return m_ShaderType;
    }

    Ref<UniformBuffer> GLShader::GetUniformBuffer(const std::string& name) const
    {
        auto it = m_UniformBuffers.find(name);
        if (it != m_UniformBuffers.end())
        {
            return std::reinterpret_pointer_cast<UniformBuffer>(it->second);
        }

        return nullptr;
    }

    Ref<ShaderStorageBuffer> GLShader::GetShaderStorageBuffer(const std::string& name) const
    {
        auto it = m_SSOBBuffers.find(name);
        if (it != m_SSOBBuffers.end())
        {
            return std::reinterpret_pointer_cast<ShaderStorageBuffer>(it->second);
        }

        return nullptr;
    }

    Ref<SamplerState> GLShader::GetSampler(const std::string& name) const
    {
        auto it = m_Samplers.find(name);
        if (it != m_Samplers.end())
        {
            return std::reinterpret_pointer_cast<SamplerState>(it->second);
        }

        return nullptr;
    }

    Ref<SamplerState> GLShader::GetImage(const std::string& name) const
    {
        auto it = m_Samplers.find(name);
        if (it != m_Samplers.end())
        {
            return std::reinterpret_pointer_cast<SamplerState>(it->second);
        }

        return nullptr;
    }
}
