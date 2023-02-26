#include "boidpch.h"
#include "GLRenderTarget.h"


namespace Polyboid
{
    GLRenderTarget::GLRenderTarget(const RenderTargetSettings& settings): m_Settings(settings)
    {
        FramebufferSettings framebufferSettings;
        framebufferSettings.height = settings.Height;
        framebufferSettings.width = settings.Width;

        m_Framebuffer = std::make_shared<GLFramebuffer>(framebufferSettings);

        for (auto& element : m_Settings.TextureAttachments)
        {
            TextureSettings textureSettings;
            textureSettings.Width = settings.Width;
            textureSettings.Height = settings.Height;
            textureSettings.sizedFormat = element.format;
            auto texture = Texture::Create(textureSettings);
            m_Framebuffer->AttachTexture(texture, element.slot);
        }
       

    }

    void GLRenderTarget::AttachTexture(TextureAttachmentSlot attachment, Ref<Texture> texture)
    {
        m_Framebuffer->AttachTexture(texture, attachment);
    }

    Ref<Texture> GLRenderTarget::GetTexture(TextureAttachmentSlot attachment)
    {
        return m_Framebuffer->GetTexture(attachment);
    }

    void GLRenderTarget::Clear(TextureAttachmentSlot attachment, const ClearSettings& settings)
    {
        m_Framebuffer->Clear(attachment, settings);
    }

    void GLRenderTarget::Clear(const ClearSettings& settings)
    {
        m_Framebuffer->Clear(settings);
    }

    void GLRenderTarget::GenerateMipMaps()
    {
    }

    void GLRenderTarget::Bind()
    {
        m_Framebuffer->Bind();
    }

    void GLRenderTarget::UnBind()
    {
        m_Framebuffer->UnBind();
    }
}
