#include "boidpch.h"
#include "GLTexture3D.h"

#include "Utils.h"
#include "glad/glad.h"

namespace Polyboid
{
    GLTexture3D::GLTexture3D(const void** data, const TextureSettings& settings): m_Settings(settings)
    {
        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_Handle);
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
       
        for (int face = 0; face < 6; ++face)
        {
            glTextureSubImage3D(m_Handle, 0, 0, 0, face, settings.Width, settings.Height, 1,
                Utils::ConvertTextureFormatToOpenGL(settings.textureFormat),
                Utils::ConvertOpenGLTextureDataType(settings.textureData), data);
        }
        
        if (settings.generateMips)
        {
            glGenerateTextureMipmap(m_Handle);
        }

        
    }

    void GLTexture3D::Bind(uint32_t slot)
    {
        glBindTextureUnit(slot, m_Handle);
    }

    void GLTexture3D::UnBind()
    {
        glBindTextureUnit(0, 0);
    }

    void GLTexture3D::SetData(const void** data, uint32_t size)
    {
        __debugbreak();
    }

    GLTexture3D::~GLTexture3D()
    {
        glDeleteTextures(1, &m_Handle);
    }
}
