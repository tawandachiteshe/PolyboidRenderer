#pragma once
#include "Shader.h"
#include <map>

#include "Engine/Engine/ShaderRegistry/ShaderRegistry.h"


namespace Polyboid
{
	class Image2D;
	class RenderPass;
	class Material;
	class TexelStorageBuffer;
	class TexelUniformBuffer;
	enum class PrimitiveType;
	class StorageBufferSet;
	class UniformBufferSet;
	class PipelineDescriptorSet;
	class PipelineDescriptorSetPool;
	class MultiSampleState;
	class VertexBufferArray;
	class BlendState;
    class RasterizerState;
    class DepthStencilState;

    class GraphicsPipeline : public RenderResource
    {
    public:
        using ShaderMap = std::map<ShaderType, Ref<Shader>>;

        virtual void SetShader(const ShaderType& type, const Ref<Shader>& shader) = 0;
        virtual void SetShader(const Ref<Shader>& shader) = 0;
        virtual Ref<Shader> GetShader( const ShaderType& type ) const = 0;
        [[nodiscard]] virtual const ShaderMap& GetShaders() const = 0;

        virtual Ref<Material> CreateMaterial(const std::string& name) = 0;

        virtual void SetGraphicsShaders(const GraphicsShaders& shaders) = 0;

        virtual void SetBlendState( const BlendState& blendState ) = 0;
        virtual BlendState& GetBlendState() = 0;

        virtual void SetRasterizerState( const RasterizerState& rasterizerState ) = 0;
        virtual RasterizerState& GetRasterizerState() = 0;

        virtual void SetDepthStencilState( const DepthStencilState& depthStencilState ) = 0;
        virtual DepthStencilState& GetDepthStencilState() = 0;

        virtual void SetMultiSampleState(const MultiSampleState& multiSampleState) = 0;
        virtual MultiSampleState& GetMutltiSampleState() = 0;

        virtual void SetVertexArray(const Ref<VertexBufferArray>& va) = 0;
        virtual Ref<VertexBufferArray> GetVertexArray() = 0;

        virtual void SetRenderPass(const Ref<RenderPass>& renderPass) = 0;
        virtual  Ref<RenderPass> GetRenderPass() = 0;

        virtual  ~GraphicsPipeline() = default;

        virtual void Bake() = 0;
        virtual std::any GetHandle() = 0;
        
        virtual void Bind() = 0;
        virtual void UnBind() = 0;

        virtual void SetGeometryTopology(const PrimitiveType& primitiveType) = 0;

        virtual void BindUniformBufferSet(uint32_t binding, const Ref<UniformBufferSet>& bufferSet, uint32_t setBinding = 0) = 0;
        virtual void BindStorageBufferSet(uint32_t binding, const Ref<StorageBufferSet>& bufferSet, uint32_t setBinding = 0) = 0;
        virtual void BindTexture2D(uint32_t binding, const Ref<Texture2D>& bufferSet, uint32_t setBinding = 0) = 0;
        virtual void BindTexture3D(uint32_t binding, const Ref<Texture3D>& texture, uint32_t setBinding = 0) = 0;
        virtual void BindImage2D(uint32_t binding, const Ref<Image2D>& bufferSet, uint32_t setBinding) = 0;
        virtual void BindTexelUniformBuffer(uint32_t binding, const Ref<TexelUniformBuffer>& bufferSet, uint32_t setBinding) = 0;
        virtual void BindTexelStorageBuffer(uint32_t binding, const Ref<TexelStorageBuffer>& bufferSet, uint32_t setBinding) = 0;
        virtual void BindResource(const std::string& name, const Ref<RenderResource>& resource) = 0;


        virtual void WriteSetResourceBindings(uint32_t set = 0) = 0;


        virtual std::vector<Ref<PipelineDescriptorSet>> AllocateDescriptorSets(uint32_t setBinding = 0) = 0;
        virtual std::vector<Ref<PipelineDescriptorSet>> GetDescriptorSets(uint32_t set) = 0;

        static Ref<GraphicsPipeline> Create();

    };

}

