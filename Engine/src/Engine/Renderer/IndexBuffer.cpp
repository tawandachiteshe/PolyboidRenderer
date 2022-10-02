#include "IndexBuffer.h"

#include "glad/glad.h"


namespace Polyboid
{
    IndexBuffer::IndexBuffer(const uint32_t* indices, uint32_t count) : m_Count(count)
    {
        glGenBuffers(1, &m_ID);
        glBindBuffer(GL_ARRAY_BUFFER, m_ID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(uint32_t) * count, indices, GL_STATIC_DRAW);
    }

    IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers(1, &m_ID);
    }

    void IndexBuffer::Bind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
    }

    std::shared_ptr<IndexBuffer> IndexBuffer::MakeIndexBuffer(const uint32_t* indices, uint32_t count)
    {
        return std::make_shared<IndexBuffer>(indices, count);        
    }
}
