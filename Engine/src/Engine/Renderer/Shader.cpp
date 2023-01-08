#include "boidpch.h"

#include "Shader.h"

#include <spdlog/spdlog.h>

#include "Engine/Engine/Base.h"
#include "Engine/Engine/FileReader.h"
#include "Engine/Engine/Debug/Profiler.h"
#include "glad/glad.h"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"



namespace Polyboid
{
    void Shader::PrepareShaders(const char* vertexSrc, const char* fragSrc)
    {
        POLYBOID_PROFILE_FUNCTION();

        uint32_t fragShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragShader, 1, &fragSrc, NULL);
        glCompileShader(fragShader);
        CheckErrors(fragShader, "FRAGMENT");

        uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexSrc, NULL);
        glCompileShader(vertexShader);
        CheckErrors(vertexShader, "VERTEX");

        m_Program = glCreateProgram();
        
        glAttachShader(m_Program, vertexShader);
        glAttachShader(m_Program, fragShader);


        glLinkProgram(m_Program);
        glValidateProgram(m_Program);
        CheckErrors(m_Program, "PROGRAM");


        glDetachShader(m_Program, vertexShader);
        glDetachShader(m_Program, fragShader);

        glDeleteShader(vertexShader);
        glDeleteShader(fragShader);
    }

    void Shader::CheckErrors(uint32_t shader, const char* type)
    {
        POLYBOID_PROFILE_FUNCTION();

        int32_t hasCompiled = 0;

        char infoLog[1024];
        int InfoLogLength = 0;

        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &InfoLogLength);

            if (hasCompiled == GL_FALSE)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                glDeleteShader(shader);

                spdlog::error("SHADER COMPILATION ERROR for {0}: {1}", type, infoLog);
            }
        }
        else
        {
            GLint isLinked = 0;

            glGetProgramiv(shader, GL_LINK_STATUS, &isLinked);

            if (isLinked == GL_FALSE)
            {
                glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &InfoLogLength);
                glGetProgramInfoLog(shader, 1024, nullptr, &infoLog[0]);

                glDeleteProgram(shader);
                
                
                spdlog::error("PROGRAM LINKING ERROR for {0}: {1}", type, infoLog);
            }
        }
    }


    Shader:: Shader(const std::string& vertexPath, const std::string& fragPath)
    {
        POLYBOID_PROFILE_FUNCTION();

        //read files here
        const std::string vertexSrc = FileReader::ReadString(vertexPath);
        const std::string fragSrc = FileReader::ReadString(fragPath);


        if (vertexSrc.empty())
        {
            spdlog::error("Vertex Shader src not found path {}", vertexPath);
        }

        if (fragPath.empty())
        {

            spdlog::error("Pixel Shader src not found path {}", fragPath);
        }

        PrepareShaders(vertexSrc.c_str(), fragSrc.c_str());
    }

    Shader::Shader(const std::string& computePath)
    {
        const std::string vertexSrc = FileReader::ReadString(computePath);

        uint32_t computeShader;

        computeShader = glCreateShader(GL_COMPUTE_SHADER);
        const char* src = vertexSrc.c_str();
        glShaderSource(computeShader, 1, &src, nullptr);
        glCompileShader(computeShader);
        CheckErrors(computeShader, "COMPUTE");

        m_Program = glCreateProgram();
        glAttachShader(m_Program, computeShader);
        glLinkProgram(m_Program);
        CheckErrors(m_Program, "PROGRAM");

    }

    Shader::~Shader()
    {
        POLYBOID_PROFILE_FUNCTION();

        glDeleteProgram(m_Program);
    }


    void Shader::Bind()
    {
        POLYBOID_PROFILE_FUNCTION();

        glUseProgram(m_Program);
    }

    Ref<Shader> Shader::MakeShader(const std::string& vertexPath, const std::string& fragPath)
    {
        POLYBOID_PROFILE_FUNCTION();

        return std::make_shared<Shader>(vertexPath, fragPath);
    }

    Ref<Shader> Shader::MakeShader(const std::string& computePath)
    {
        return std::make_shared<Shader>(computePath);
    }

    void Shader::SetInt(const std::string& name, int value)
    {
        POLYBOID_PROFILE_FUNCTION();

        GLint location = glGetUniformLocation(m_Program, name.c_str());
        glUniform1i(location, value);
    }

    void Shader::SetIntArray(const std::string& name, int* values, uint32_t count)
    {
        POLYBOID_PROFILE_FUNCTION();

        GLint location = glGetUniformLocation(m_Program, name.c_str());
        glUniform1iv(location, count, values);
    }

    void Shader::SetFloat(const std::string& name, float value)
    {
        POLYBOID_PROFILE_FUNCTION();

        GLint location = glGetUniformLocation(m_Program, name.c_str());
        glUniform1f(location, value);
    }

    void Shader::SetFloat2(const std::string& name, const glm::vec2& value)
    {
        POLYBOID_PROFILE_FUNCTION();

        GLint location = glGetUniformLocation(m_Program, name.c_str());
        glUniform2f(location, value.x, value.y);
    }

    void Shader::SetFloat3(const std::string& name, const glm::vec3& value)
    {
        POLYBOID_PROFILE_FUNCTION();

        GLint location = glGetUniformLocation(m_Program, name.c_str());
        glUniform3f(location, value.x, value.y, value.z);
    }

    void Shader::SetFloat4(const std::string& name, const glm::vec4& value)
    {
        POLYBOID_PROFILE_FUNCTION();

        GLint location = glGetUniformLocation(m_Program, name.c_str());
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    void Shader::SetMat3(const std::string& name, const glm::mat3& value)
    {
        POLYBOID_PROFILE_FUNCTION();

        GLint location = glGetUniformLocation(m_Program, name.c_str());
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

    void Shader::SetMat4(const std::string& name, const glm::mat4& value)
    {
        POLYBOID_PROFILE_FUNCTION();

        GLint location = glGetUniformLocation(m_Program, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }
}
