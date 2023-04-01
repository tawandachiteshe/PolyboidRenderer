#pragma once
#include "Shader.h"
#include <map>

#include "RenderPass.h"


namespace Polyboid
{
	class VertexBufferArray;
	class BlendState;
    class RasterizerState;
    class DepthStencilState;

    class PipelineState
    {
    public:
        using ShaderMap = std::map<ShaderType, Ref<Shader>>;

        virtual void SetShader(const ShaderType& type, const Ref<Shader>& shader) = 0;
        virtual void SetShader(const Ref<Shader>& shader) = 0;
        virtual std::shared_ptr<Shader> GetShader( const ShaderType& type ) const = 0;
        [[nodiscard]] virtual const ShaderMap& GetShaders() const = 0;

        virtual void SetBlendState( const BlendState& blendState ) = 0;
        virtual BlendState& GetBlendState() = 0;

        virtual void SetRasterizerState( const RasterizerState& rasterizerState ) = 0;
        virtual RasterizerState& GetRasterizerState() = 0;

        virtual void SetDepthStencilState( const DepthStencilState& depthStencilState ) = 0;
        virtual DepthStencilState& GetDepthStencilState() = 0;

        virtual void SetVertexArray(const Ref<VertexBufferArray>& va) = 0;
        virtual Ref<VertexBufferArray>& GetVertexArray() = 0;
        
        virtual  ~PipelineState() = default;
        
        virtual void Bind() = 0;
        virtual void UnBind() = 0;
    };

}

