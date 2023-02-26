#include "boidpch.h"
#include "GLIndexBuffer.h"

#include "glad/glad.h"

namespace Polyboid
{
    GLIndexBuffer::GLIndexBuffer(const IndexDataType& type, uint32_t count, const std::variant<uint32_t*, uint16_t*>& data):
        m_DataType(type), m_Count(count)
    {
        glCreateBuffers(1, &m_Handle);

        switch (type)
        {
        case IndexDataType::UnsignedShort:
            glNamedBufferStorage(m_Handle, sizeof(uint16_t) * count, std::get<uint16_t*>(data), GL_DYNAMIC_STORAGE_BIT);
            break;
        case IndexDataType::UnsignedInt:
            glNamedBufferStorage(m_Handle, sizeof(uint32_t) * count, std::get<uint32_t*>(data), GL_DYNAMIC_STORAGE_BIT);
            break;
        }
    }

    GLIndexBuffer::~GLIndexBuffer()
    {
        glDeleteBuffers(1, &m_Handle);
    }

    void GLIndexBuffer::Bind() const
    {
    }

    void GLIndexBuffer::Unbind() const
    {
    }

    uint32_t GLIndexBuffer::GetCount() const
    {
        return m_Count;
    }

    IndexDataType GLIndexBuffer::GetIndexDataType()
    {
        return m_DataType;
    }
}
