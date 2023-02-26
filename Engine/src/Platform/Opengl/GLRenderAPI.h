#pragma once
#include <any>

#include "Engine/Renderer/RenderAPI.h"


namespace Polyboid
{
    
    class GLRenderAPI : public RenderAPI
    {
    public:

        GLRenderAPI(const std::any& windowHandle);
        
        void DrawIndexed(const PrimitiveType& primitiveType, const IndexDataType& indexDataType,
            uint32_t count) override;
        void DrawArrays(const PrimitiveType& primitiveType, uint32_t vertexCount) override;

        void Dispatch(const glm::uvec3& groups) override;

        Ref<Texture> CreateTexture2D(const TextureSettings& settings) override;
        Ref<Texture3D> CreateTexture3D(const void** data, const TextureSettings& settings) override;
        Ref<UniformBuffer> CreateUniformBuffer(uint32_t size, uint32_t binding) override;
        Ref<Framebuffer> CreateFrameBuffer(const FramebufferSettings& settings) override;
        Ref<IndexBuffer> CreateIndexBuffer(const IndexDataType& type, uint32_t count,
	        const std::variant<uint32_t*, uint16_t*>& data) override;

        Ref<SamplerState> CreateSampler(const SamplerSettings& settings) override;
        Ref<Renderbuffer> CreateRenderBuffer(const RenderbufferSettings& settings) override;
        Ref<VertexBuffer> CreateVertexBuffer(const void* data, uint32_t dataSize) override;
        Ref<VertexBuffer> CreateVertexBuffer(uint32_t dataSize) override;
        Ref<VertexBufferArray> CreateVertexBufferArray() override;

        Ref<PipelineState> CreatePipelineState() override;
        Ref<Swapchain> CreateSwapChain(const std::any& window) override;
        Ref<RenderTarget> CreateRenderTarget(const RenderTargetSettings& settings) override;


        void BeginRenderPass(const Ref<RenderTarget>& renderTarget) override;
        void EndRenderPass(const Ref<RenderTarget>& renderTarget) override;

        RenderAPIType GetRenderAPIType() override;
        ~GLRenderAPI() override = default;
    };

}


