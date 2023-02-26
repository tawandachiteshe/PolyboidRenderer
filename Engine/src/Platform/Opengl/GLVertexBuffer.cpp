#include "boidpch.h"
#include "GLVertexBuffer.h"

#include "glad/glad.h"

namespace Polyboid
{
    GLVertexbuffer::GLVertexbuffer(const void* data, uint32_t dataSize)
    {
        glCreateBuffers(1, &m_Handle);
        glNamedBufferStorage(m_Handle, dataSize, data, GL_DYNAMIC_STORAGE_BIT);
    }

    GLVertexbuffer::GLVertexbuffer(uint32_t dataSize)
    {

        auto _dataSize = dataSize;

        glCreateBuffers(1, &m_Handle);
        glNamedBufferStorage(m_Handle, dataSize, 0, GL_DYNAMIC_STORAGE_BIT);
    }

    GLVertexbuffer::~GLVertexbuffer()
    {
        glDeleteBuffers(1, &m_Handle);
    }

    void GLVertexbuffer::Bind() const
    {
    }

    void GLVertexbuffer::Unbind() const
    {
    }

    void GLVertexbuffer::SetData(const void* data, uint32_t size)
    {
        glNamedBufferSubData(m_Handle, 0, size, data);
    }

    const BufferLayout& GLVertexbuffer::GetLayout() const
    {
        return m_Layout;
    }

    void GLVertexbuffer::SetLayout(const BufferLayout& layout)
    {
        m_Layout = layout;
    }
}
