#pragma once
#include "GLFramebuffer.h"
#include "Engine/Renderer/RenderPass.h"


namespace Polyboid
{
    class GLRenderPass : public RenderPass
    {
    private:
        Ref<GLFramebuffer> m_Framebuffer;
        RenderPassSettings m_Settings;
    public:

        GLRenderPass(const RenderPassSettings& settings);
        void SetFramebuffer(const Ref<Framebuffer>& framebuffer) override;
        Ref<Framebuffer> GetFramebuffer() override;


        void AttachTexture(TextureAttachmentSlot attachment, Ref<Texture> texture) override;
        Ref<Texture> GetTexture(TextureAttachmentSlot attachment) override;
        void Clear(TextureAttachmentSlot attachment, const ClearSettings& settings) override;
        void Clear(const ClearSettings& settings) override;
        void GenerateMipMaps() override;


        virtual void Bind();
        virtual void UnBind();

        ~GLRenderPass() override = default;
    };

}

