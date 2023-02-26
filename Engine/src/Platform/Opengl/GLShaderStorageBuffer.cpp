#include "boidpch.h"
#include "GLShaderStorageBuffer.h"
#include <glad/glad.h>

namespace Polyboid
{
    GLShaderStorageBuffer::GLShaderStorageBuffer(uint32_t dataSize)
    {
        glCreateBuffers(1, &m_Handle);
        glNamedBufferStorage(m_Handle, dataSize, nullptr, GL_DYNAMIC_STORAGE_BIT);
    }

    GLShaderStorageBuffer::GLShaderStorageBuffer(const void* data, uint32_t dataSize)
    {
        glCreateBuffers(1, &m_Handle);
        glNamedBufferStorage(m_Handle, dataSize, data, 0);
    }

    void GLShaderStorageBuffer::Bind(uint32_t slot) const
    {
        glBindBufferBase(
         GL_SHADER_STORAGE_BUFFER, 0, m_Handle);
    }

    void GLShaderStorageBuffer::Unbind() const
    {
        glBindBufferBase(
      GL_SHADER_STORAGE_BUFFER, 0, 0);
    }

    void GLShaderStorageBuffer::SetData(const void* data, uint32_t sizeData, uint32_t offset)
    {
        glNamedBufferSubData(m_Handle, offset, sizeData, data);
    }

    GLShaderStorageBuffer::~GLShaderStorageBuffer()
    {
        glDeleteBuffers(1, &m_Handle);
    }
}
