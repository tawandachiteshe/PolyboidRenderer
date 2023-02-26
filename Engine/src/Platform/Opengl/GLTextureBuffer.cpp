#include "boidpch.h"
#include "GLTextureBuffer.h"

#include <glad/glad.h>

#include "Utils.h"

namespace Polyboid
{
    GLTextureBuffer::GLTextureBuffer(const TextureBufferSpecification& settings): m_Settings(settings)
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_Handle);
        
        if (m_Settings.attachShaderStorage)
        {
            glCreateBuffers(1, &m_SSBOHandle);
            uint32_t size = settings.Width * settings.Height * Utils::GetPerComponentCount(settings.sizedFormat);
            glNamedBufferStorage(m_SSBOHandle, size, nullptr, GL_DYNAMIC_STORAGE_BIT);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, settings.ssboBufferSlot, m_SSBOHandle);

            glTextureBuffer(m_Handle, Utils::ConvertOpenGLTextureSizedInternalFormat(settings.sizedFormat), m_SSBOHandle);
        }
        else
        {
            glTextureStorage2D(m_Handle, 1, Utils::ConvertOpenGLTextureSizedInternalFormat(settings.sizedFormat), settings.Width, settings.Height);
        }
        
    }

    void GLTextureBuffer::Bind(uint32_t slot, uint32_t bufferSlot) const
    {
        if (bufferSlot)
        {
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bufferSlot, m_SSBOHandle);
        }
        glBindImageTexture(slot, m_Handle, 0, GL_FALSE, 0, Utils::ConvertTextureAccessToOpenGL(m_Settings.access), Utils::ConvertOpenGLTextureSizedInternalFormat(m_Settings.sizedFormat));
    }

    void GLTextureBuffer::Unbind() const
    {
    }

    GLTextureBuffer::~GLTextureBuffer()
    {
        glDeleteTextures(1, &m_Handle);
    }
}
