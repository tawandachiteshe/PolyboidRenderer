#include "boidpch.h"
#include "GLTexture2D.h"

#include "Utils.h"
#include "glad/glad.h"


namespace Polyboid
{

    GLTexture2D::GLTexture2D(const TextureSettings& settings): m_Settings(settings)
    {
        glCreateTextures(settings.multiSample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, 1, &m_Handle);
        glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_S, Utils::ConvertOpenGLTextureWrapMode(settings.wrapMode));
        glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_T, Utils::ConvertOpenGLTextureWrapMode(settings.wrapMode) );
        glTextureParameteri(m_Handle, GL_TEXTURE_MIN_FILTER, Utils::ConvertToOpenGLTextureFilterMode(settings.filterMode));
        glTextureParameteri(m_Handle, GL_TEXTURE_MAG_FILTER, Utils::ConvertToOpenGLTextureFilterMode(settings.filterMode));

        if (settings.multiSample)
        {
            glTextureStorage2DMultisample(m_Handle, settings.sampleCount, Utils::ConvertOpenGLTextureSizedInternalFormat(settings.sizedFormat), settings.Width, settings.Height, GL_FALSE);
        }
        else
        {
            glTextureStorage2D(m_Handle, 1, Utils::ConvertOpenGLTextureSizedInternalFormat(settings.sizedFormat), settings.Width, settings.Height);
        }
      
        if (settings.generateMips)
        {
            glGenerateTextureMipmap(m_Handle);
        }
    }

    GLTexture2D::GLTexture2D(const void* data, const TextureSettings& settings): m_Settings(settings)
    {
        glCreateTextures(settings.multiSample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, 1, &m_Handle);
        glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_S, Utils::ConvertOpenGLTextureWrapMode(settings.wrapMode) );
        glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_T, Utils::ConvertOpenGLTextureWrapMode(settings.wrapMode) );
        glTextureParameteri(m_Handle, GL_TEXTURE_MIN_FILTER, Utils::ConvertToOpenGLTextureFilterMode(settings.filterMode));
        glTextureParameteri(m_Handle, GL_TEXTURE_MAG_FILTER, Utils::ConvertToOpenGLTextureFilterMode(settings.filterMode));

        if (settings.multiSample)
        {
            glTextureStorage2DMultisample(m_Handle, settings.sampleCount, Utils::ConvertOpenGLTextureSizedInternalFormat(settings.sizedFormat), settings.Width, settings.Height, GL_FALSE);
        }
        else
        {
            glTextureStorage2D(m_Handle, 1, Utils::ConvertOpenGLTextureSizedInternalFormat(settings.sizedFormat), settings.Width, settings.Height);
        }
        
        glTextureSubImage2D(m_Handle, 0, 0, 0, settings.Width, settings.Height,
            Utils::ConvertTextureFormatToOpenGL(settings.textureFormat),
            Utils::ConvertOpenGLTextureDataType(settings.textureData), data);

        if (settings.generateMips)
        {
            glGenerateTextureMipmap(m_Handle);
        }
    }

    std::any GLTexture2D::GetHandle()
    {
        return m_Handle;
    }

    void GLTexture2D::SetData(const void* data, uint32_t size)
    {
        glTextureSubImage2D(m_Handle, 0, 0, 0, m_Settings.Width, m_Settings.Height,
            Utils::ConvertTextureFormatToOpenGL(m_Settings.textureFormat),
            Utils::ConvertOpenGLTextureDataType(m_Settings.textureData), data);
    }

    void GLTexture2D::Bind(uint32_t slot)
    {
        glBindTextureUnit(slot, m_Handle);
    }

    void GLTexture2D::UnBind()
    {
        glBindTextureUnit(0, 0);
    }

    GLTexture2D::~GLTexture2D()
    {
        glDeleteTextures(1, &m_Handle);
    }
}
