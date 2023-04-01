#include "boidpch.h"
#include "GLRenderPass.h"


namespace Polyboid
{
    GLRenderPass::GLRenderPass(const RenderPassSettings& settings): m_Settings(settings)
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

    void GLRenderPass::SetFramebuffer(const Ref<Framebuffer>& framebuffer)
    {
        m_Framebuffer = std::reinterpret_pointer_cast<GLFramebuffer>(framebuffer);
    }

    Ref<Framebuffer> GLRenderPass::GetFramebuffer()
    {
        return  m_Framebuffer;
    }


    void GLRenderPass::AttachTexture(TextureAttachmentSlot attachment, Ref<Texture> texture)
    {
        m_Framebuffer->AttachTexture(texture, attachment);
    }

    Ref<Texture> GLRenderPass::GetTexture(TextureAttachmentSlot attachment)
    {
        return m_Framebuffer->GetTexture(attachment);
    }

    void GLRenderPass::Clear(TextureAttachmentSlot attachment, const ClearSettings& settings)
    {
        m_Framebuffer->Clear(attachment, settings);
    }

    void GLRenderPass::Clear(const ClearSettings& settings)
    {
        m_Framebuffer->Clear(settings);
    }

    void GLRenderPass::GenerateMipMaps()
    {
    }

    void GLRenderPass::Bind()
    {
        m_Framebuffer->Bind();
    }

    void GLRenderPass::UnBind()
    {
        m_Framebuffer->UnBind();
    }
}
