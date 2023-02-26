#include "boidpch.h"
#include "GLUniformBuffer.h"

#include "glad/glad.h"

namespace Polyboid
{
    GLUniformBuffer::GLUniformBuffer(uint32_t size, uint32_t binding): m_Slot(binding), m_DataSize(size)
    {
        glCreateBuffers(1, &m_Handle);
        glNamedBufferStorage(m_Handle, size, nullptr, GL_DYNAMIC_STORAGE_BIT);
        glBindBufferRange(GL_UNIFORM_BUFFER, binding, m_Handle, 0, size);
        
    }

    uint32_t GLUniformBuffer::GetBindingSlot()
    {
        return m_Slot;
    }

    uint32_t GLUniformBuffer::GetDataSize()
    {
        return m_DataSize;
    }

    void GLUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
    {
        glNamedBufferSubData(m_Handle, offset, size, data);
    }

    GLUniformBuffer::~GLUniformBuffer()
    {
        glDeleteBuffers(1, &m_Handle);
    }
}
