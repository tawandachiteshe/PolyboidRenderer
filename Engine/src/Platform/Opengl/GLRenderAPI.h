#pragma once
#include <any>

#include "Engine/Renderer/RenderAPI.h"


namespace Polyboid
{
    
    class GLRenderAPI : public RenderAPI
    {

    private:

        std::any m_Window;

    public:
        void BeginFrame() override;
        void EndFrame() override;
        GLRenderAPI(const std::any& windowHandle);

        Ref<CommandList> CreateCommandList() override;

        Ref<Texture> CreateTexture2D(const TextureSettings& settings) override;
        Ref<Texture3D> CreateTexture3D(const void** data, const TextureSettings& settings) override;
        Ref<UniformBuffer> CreateUniformBuffer(uint32_t size, uint32_t binding) override;
        Ref<Framebuffer> CreateFrameBuffer(const FramebufferSettings& settings) override;
        Ref<IndexBuffer> CreateIndexBuffer(const IndexDataType& type, uint32_t count,
	        const std::variant<uint32_t*, uint16_t*>& data) override;

        Ref<Texture> CreateTexture2D(const std::any& handle) override;
        Ref<Framebuffer>
        CreateFrameBuffer(const FramebufferSettings& settings, const Ref<RenderPass>& renderPass) override;

        Ref<SamplerState> CreateSampler(const SamplerSettings& settings) override;
        Ref<Renderbuffer> CreateRenderBuffer(const RenderbufferSettings& settings) override;
        Ref<VertexBuffer> CreateVertexBuffer(const void* data, uint32_t dataSize) override;
        Ref<VertexBuffer> CreateVertexBuffer(uint32_t dataSize) override;
        Ref<VertexBufferArray> CreateVertexBufferArray() override;

        Ref<PipelineState> CreatePipelineState() override;
        Ref<Swapchain> CreateSwapChain(const SwapchainSettings& settings) override;
        Ref<RenderPass> CreateRenderPass(const RenderPassSettings& settings) override;

        RenderAPIType GetRenderAPIType() override;
        RenderAPIType GetRenderAPIType() const override;
        ~GLRenderAPI() override;
    };

}


