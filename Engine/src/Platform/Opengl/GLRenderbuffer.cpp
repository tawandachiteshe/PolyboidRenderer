#include "boidpch.h"
#include "GLRenderbuffer.h"

#include <glad/glad.h>

#include "Utils.h"


namespace Polyboid
{
    GLRenderbuffer::GLRenderbuffer(const RenderbufferSettings& settings): m_Settings(settings)
    {
        glCreateRenderbuffers(1, &m_Handle);
        glNamedRenderbufferStorage(m_Handle, Utils::ConvertOpenGLTextureSizedInternalFormat(settings.format), settings.width, settings.height);
    }

    RenderbufferSettings GLRenderbuffer::GetSettings()
    {
        return m_Settings;
    }

    GLRenderbuffer::~GLRenderbuffer()
    {
        glDeleteRenderbuffers(1, &m_Handle);
    }
}
