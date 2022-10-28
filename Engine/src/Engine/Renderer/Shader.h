#pragma once
#include <cstdint>
#include <memory>
#include <string>

#include "Engine/Engine/Base.h"
#include "glm/fwd.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"


namespace Polyboid
{
    class Shader
    {
    private:
        uint32_t m_Program = 0;
        uint32_t m_Pipeline = 0;
        void PrepareShaders(const char* vertexSrc, const char* fragSrc);
        void CheckErrors(uint32_t shader, const char* type);
        
    public:
        //internals maybe
        Shader(const std::string& vertexPath, const std::string& fragPath);
        ~Shader();
        
        void Bind();
        static  Ref<Shader> MakeShader(const std::string& vertexPath, const std::string& fragPath);

        void SetInt(const std::string& name, int value);
        void SetIntArray(const std::string& name, int* values, uint32_t count);
        void SetFloat(const std::string& name, float value);
        void SetFloat2(const std::string& name,  const glm::vec2& value);
        void SetFloat3(const std::string& name,  const glm::vec3& value);
        void SetFloat4(const std::string& name,  const glm::vec4& value);
        void SetMat3(const std::string& name, const glm::mat3& value);
        void SetMat4(const std::string& name, const glm::mat4& value);
  
    
    };


}
