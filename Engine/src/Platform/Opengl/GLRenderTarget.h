#pragma once
#include "GLFramebuffer.h"
#include "Engine/Renderer/RenderTarget.h"


namespace Polyboid
{
    class GLRenderTarget : public RenderTarget
    {
    private:
        Ref<GLFramebuffer> m_Framebuffer;
        RenderTargetSettings m_Settings;
    public:

        GLRenderTarget(const RenderTargetSettings& settings);
        
        void AttachTexture(TextureAttachmentSlot attachment, Ref<Texture> texture) override;
        Ref<Texture> GetTexture(TextureAttachmentSlot attachment) override;
        void Clear(TextureAttachmentSlot attachment, const ClearSettings& settings) override;
        void Clear(const ClearSettings& settings) override;
        void GenerateMipMaps() override;


        virtual void Bind();
        virtual void UnBind();

        ~GLRenderTarget() override = default;
    };

}

