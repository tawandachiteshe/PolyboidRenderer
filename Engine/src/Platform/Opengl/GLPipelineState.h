#pragma once
#include "GLBlendState.h"
#include "GLDepthStencilState.h"
#include "GLRasterizerState.h"
#include "Engine/Renderer/PipelineState.h"


namespace Polyboid
{
	class GLRenderTarget;

	class GLPipelineState : public PipelineState
    {
    private:
        uint32_t m_Handle = 0;
        ShaderMap m_Shaders;

        GLBlendState m_BlendState;
        GLRasterizerState m_RasterizerState;
        GLDepthStencilState m_DepthStencilState;
        Ref<GLRenderTarget> m_RenderTarget;
        Ref<VertexBufferArray> m_VertexBufferArray;
        
        
        
    public:
        GLPipelineState();
        
        void SetShader(const ShaderType& type, const Ref<Shader>& shader) override;
        void SetShader(const Ref<Shader>& shader) override;
        [[nodiscard]] Ref<Shader> GetShader(const ShaderType& type) const override;

        [[nodiscard]] const ShaderMap& GetShaders() const override;
        void SetBlendState(const BlendState& blendState) override;

        BlendState& GetBlendState() override;
        void SetRasterizerState(const RasterizerState& rasterizerState) override;

        RasterizerState& GetRasterizerState() override;
        void SetDepthStencilState(const DepthStencilState& depthStencilState) override;

        DepthStencilState& GetDepthStencilState() override;
    	~GLPipelineState() override;


        void SetVertexArray(const Ref<VertexBufferArray>& va) override;
        Ref<VertexBufferArray>& GetVertexArray() override;


        void Bind() override;
        void UnBind() override;

       
    
    };

}

