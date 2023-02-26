#include "boidpch.h"
#include "GLFramebuffer.h"

#include "GLRenderbuffer.h"
#include "GLTexture2D.h"
#include "Engine/Renderer/RenderTarget.h"
#include "glad/glad.h"
#include "glm/gtc/type_ptr.inl"

namespace Polyboid
{

    GLenum GetOpenglAttachmentPoint(const TextureAttachmentSlot& slot)
    {
        switch (slot)
        {
        case TextureAttachmentSlot::Color0: return GL_COLOR_ATTACHMENT0;
        case TextureAttachmentSlot::Color1: return GL_COLOR_ATTACHMENT1;
        case TextureAttachmentSlot::Color2: return GL_COLOR_ATTACHMENT2;
        case TextureAttachmentSlot::Color3: return GL_COLOR_ATTACHMENT3;
        case TextureAttachmentSlot::Color4: return GL_COLOR_ATTACHMENT4;
        case TextureAttachmentSlot::Color5: return GL_COLOR_ATTACHMENT5;
        case TextureAttachmentSlot::Color6: return GL_COLOR_ATTACHMENT6;
        case TextureAttachmentSlot::Color7: return GL_COLOR_ATTACHMENT7;
        case TextureAttachmentSlot::Depth: return GL_DEPTH_ATTACHMENT;
        case TextureAttachmentSlot::Stencil: return GL_STENCIL_ATTACHMENT;
        case TextureAttachmentSlot::DepthStencil: return GL_DEPTH_STENCIL_ATTACHMENT;
        }

        return 0;
    }

    bool IsColorAttachent(const TextureAttachmentSlot& slot)
    {
        switch (slot)
        {
        case TextureAttachmentSlot::Color0:
        case TextureAttachmentSlot::Color1:
        case TextureAttachmentSlot::Color2:
        case TextureAttachmentSlot::Color3:
        case TextureAttachmentSlot::Color4:
        case TextureAttachmentSlot::Color5:
        case TextureAttachmentSlot::Color6:
        case TextureAttachmentSlot::Color7:
            return true;
        case TextureAttachmentSlot::Depth:
        case TextureAttachmentSlot::Stencil:
        case TextureAttachmentSlot::DepthStencil:
            return false;
        }

        return false;
    }

    GLFramebuffer::GLFramebuffer(const FramebufferSettings& settings): m_Settings(settings)
    {
        glCreateFramebuffers(1, &m_Handle);

        constexpr uint32_t TextureAttachmentSlotsCount = 11;
    }

    void GLFramebuffer::AttachRenderbuffer(const Ref<Renderbuffer>& renderbuffer, const TextureAttachmentSlot& slot)
    {
        m_Renderbuffers[slot] = std::dynamic_pointer_cast<GLRenderbuffer>(renderbuffer);


    }

    void GLFramebuffer::AttachTexture(const Ref<Texture>& texture, const TextureAttachmentSlot& slot)
    {
       
        m_Textures[slot] = std::dynamic_pointer_cast<GLTexture2D>(texture);

    }

    void GLFramebuffer::AttachTexture3D(const Ref<Texture3D>& texture, const TextureAttachmentSlot& slot)
    {
        if (!m_Textures.empty())
        {
            std::vector<GLenum> m_Colors;

            for (auto& textures : m_Textures)
            {
                glNamedFramebufferTextureLayer(m_Handle, GetOpenglAttachmentPoint(textures.first), textures.second->m_Handle, 0, 0);
                m_Colors.push_back(GetOpenglAttachmentPoint(textures.first));

            }



            glNamedFramebufferDrawBuffers(m_Handle, m_Colors.size(), m_Colors.data());
        }

        if (glCheckNamedFramebufferStatus(m_Handle, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            //TODO: Platform agnostic
            __debugbreak();
        }

        
    }

    void GLFramebuffer::Bind()
    {
	    if (m_Dirty)
	    {

            GLenum buffers[4] = { GL_COLOR_ATTACHMENT0,  GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };

            //Frame buffers
            if (!m_Textures.empty())
            {

                uint32_t count = 0;
                for (auto& textures : m_Textures)
                {
                    glNamedFramebufferTexture(m_Handle, GetOpenglAttachmentPoint(textures.first), textures.second->m_Handle, 0);
                    if (IsColorAttachent(textures.first))
                    {
                        count++;
                    }
                }

                if (count)
                {
                    glNamedFramebufferDrawBuffers(m_Handle, count, buffers);
                }
                else
                {
                    glNamedFramebufferDrawBuffer(m_Handle, GL_NONE);

                }
                
            }

            if (glCheckNamedFramebufferStatus(m_Handle, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            {
                //TODO: Platform agnostic
                __debugbreak();
            }

            //Render Buffers
            if (!m_Renderbuffers.empty())
            {
                std::vector<GLenum> m_Colors;

                uint32_t count = 0;
                for (auto& renderbuffer : m_Renderbuffers)
                {
                    glNamedFramebufferRenderbuffer(m_Handle, GetOpenglAttachmentPoint(renderbuffer.first), GL_RENDERBUFFER, renderbuffer.second->m_Handle);
                    if (IsColorAttachent(renderbuffer.first))
                    {
                        count++;
                    }
                }


                if (count)
                {
                    glNamedFramebufferDrawBuffers(m_Handle, count, buffers);
                }
                else
                {
                    glNamedFramebufferDrawBuffer(m_Handle, GL_NONE);

                }

            }

            if (glCheckNamedFramebufferStatus(m_Handle, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            {
                //TODO: Platform agnostic
                __debugbreak();
            }

            m_Dirty = false;
	    }

        glBindFramebuffer(GL_FRAMEBUFFER, m_Handle);
        
    }

    void GLFramebuffer::UnBind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    Ref<Texture> GLFramebuffer::GetTexture(TextureAttachmentSlot slot)
    {
        return m_Textures[slot];
    }

    void GLFramebuffer::ClearAllColors(const ClearSettings& settings)
    {
	    for (auto& texture : m_Textures)
	    {
		    switch (texture.first)
		    {
		    case TextureAttachmentSlot::Color0:  glClearNamedFramebufferfv(m_Handle, GL_COLOR, 0, glm::value_ptr(settings.color)); break;
            case TextureAttachmentSlot::Color1: glClearNamedFramebufferfv(m_Handle, GL_COLOR, 1, glm::value_ptr(settings.color));  break;
            case TextureAttachmentSlot::Color2: glClearNamedFramebufferfv(m_Handle, GL_COLOR, 2, glm::value_ptr(settings.color)); break;
		    case TextureAttachmentSlot::Color3: glClearNamedFramebufferfv(m_Handle, GL_COLOR, 3, glm::value_ptr(settings.color)); break;
            case TextureAttachmentSlot::Color4: glClearNamedFramebufferfv(m_Handle, GL_COLOR, 4, glm::value_ptr(settings.color)); break;
            case TextureAttachmentSlot::Color5: glClearNamedFramebufferfv(m_Handle, GL_COLOR, 5, glm::value_ptr(settings.color)); break;
            case TextureAttachmentSlot::Color6: glClearNamedFramebufferfv(m_Handle, GL_COLOR, 6, glm::value_ptr(settings.color)); break;
            case TextureAttachmentSlot::Color7: glClearNamedFramebufferfv(m_Handle, GL_COLOR, 7, glm::value_ptr(settings.color)); break;
		    case TextureAttachmentSlot::Depth:  glClearNamedFramebufferfv(m_Handle, GL_DEPTH, 0, &settings.depth); break;
		    case TextureAttachmentSlot::Stencil:  glClearNamedFramebufferiv(m_Handle, GL_STENCIL, 0, (GLint*)(&settings.stencil)); break;
		    case TextureAttachmentSlot::DepthStencil:glClearNamedFramebufferfi(m_Handle, GL_DEPTH_STENCIL, 0, settings.depth, settings.stencil); break;
		    }
	    }
    }

    void GLFramebuffer::Clear(TextureAttachmentSlot attachment, const ClearSettings& settings)
    {
        switch (settings.clearFlags)
        {
        case ClearFlags::Color:
            glClearNamedFramebufferfv(m_Handle, GL_COLOR, static_cast<GLint>(attachment), glm::value_ptr(settings.color));
            break;
        case ClearFlags::Depth:
            glClearNamedFramebufferfv(m_Handle, GL_DEPTH, 0, &settings.depth);
            break;
        case ClearFlags::Stencil:
            glClearNamedFramebufferiv(m_Handle, GL_STENCIL, 0, (GLint*)(&settings.stencil));
            break;
        case ClearFlags::DepthStencil:
            glClearNamedFramebufferfi(m_Handle, GL_DEPTH_STENCIL, 0, settings.depth, settings.stencil);
            break;
        case ClearFlags::All:
            glClearNamedFramebufferfv(m_Handle, GL_COLOR, static_cast<GLint>(attachment), glm::value_ptr(settings.color));
            glClearNamedFramebufferfi(m_Handle, GL_DEPTH_STENCIL, 0, settings.depth, settings.stencil);
            break;
        }
        
    }

    void GLFramebuffer::Clear(const ClearSettings& settings)
    {
        ClearAllColors(settings);
    }

    GLFramebuffer::~GLFramebuffer()
    {
        glDeleteFramebuffers(1, &m_Handle);
    }
}
