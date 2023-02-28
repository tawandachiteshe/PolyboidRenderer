#include "boidpch.h"
#include "GLPipelineState.h"
#include "GLShader.h"
#include "GLRasterizerState.h"
#include "GLRenderTarget.h"

#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/RasterizerState.h"
#include "Engine/Renderer/VertexBufferArray.h"
#include "glad/glad.h"


namespace Polyboid
{
    GLPipelineState::GLPipelineState()
    {
        glCreateProgramPipelines(1, &m_Handle);
        
    }

    void GLPipelineState::SetShader(const ShaderType& type, const Ref<Shader>& shader)
    {
        uint32_t shaderBit = 0;
        switch (type)
        {
            case ShaderType::Vertex: shaderBit = GL_VERTEX_SHADER_BIT; break;
            case ShaderType::Fragment: shaderBit = GL_FRAGMENT_SHADER_BIT ;  break;
            case ShaderType::Compute: shaderBit = GL_COMPUTE_SHADER_BIT;  break;
            case ShaderType::None: __debugbreak();
        }

        const auto glShader = std::reinterpret_pointer_cast<GLShader>(shader);
        glUseProgramStages(m_Handle, shaderBit, glShader->m_ShaderProgram);

        m_Shaders[type] = shader;
        
    }

    void GLPipelineState::SetShader(const Ref<Shader>& shader)
    {
        auto type = shader->GetType();
        uint32_t shaderBit = 0;
        switch (type)
        {
        case ShaderType::Vertex: shaderBit = GL_VERTEX_SHADER_BIT; break;
        case ShaderType::Fragment: shaderBit = GL_FRAGMENT_SHADER_BIT;  break;
        case ShaderType::Compute: shaderBit = GL_COMPUTE_SHADER_BIT;  break;
        case ShaderType::None: __debugbreak();
        }

        const auto glShader = std::reinterpret_pointer_cast<GLShader>(shader);
        glUseProgramStages(m_Handle, shaderBit, glShader->m_ShaderProgram);

        m_Shaders[type] = shader;
    }

    std::shared_ptr<Shader> GLPipelineState::GetShader(const ShaderType& type) const
    {
        auto iter = m_Shaders.find( type );
        if ( iter != m_Shaders.end() )
        {
            return iter->second;
        }

        return nullptr;
    }

    const PipelineState::ShaderMap& GLPipelineState::GetShaders() const
    {
        return m_Shaders;
    }


    void GLPipelineState::SetBlendState(const BlendState& blendState)
    {
        m_BlendState = dynamic_cast<const GLBlendState&>(blendState);
    }

    BlendState& GLPipelineState::GetBlendState()
    {
        return m_BlendState;
    }

    void GLPipelineState::SetRasterizerState(const RasterizerState& rasterizerState)
    {
        m_RasterizerState = dynamic_cast<const GLRasterizerState&>(m_BlendState);
    }

    RasterizerState& GLPipelineState::GetRasterizerState()
    {
        return m_RasterizerState;
    }

    void GLPipelineState::SetDepthStencilState(const DepthStencilState& depthStencilState)
    {
        m_DepthStencilState = dynamic_cast<const GLDepthStencilState&>(depthStencilState);
    }

    DepthStencilState& GLPipelineState::GetDepthStencilState()
    {
        return m_DepthStencilState;
    }

    GLPipelineState::~GLPipelineState()
    {
        glDeleteProgramPipelines(1, &m_Handle);
    }

    void GLPipelineState::SetVertexArray(const Ref<VertexBufferArray>& va)
    {
        m_VertexBufferArray = va;
    }

    Ref<VertexBufferArray>& GLPipelineState::GetVertexArray()
    {
        return m_VertexBufferArray;
    }

    void GLPipelineState::Bind()
    {


	    if (m_VertexBufferArray)
	    {
            m_VertexBufferArray->Bind();
	    }

        m_RasterizerState.Bind();
        
        glBindProgramPipeline(m_Handle);
    }

    void GLPipelineState::UnBind()
    {
        glBindProgramPipeline(0);
    }
}
