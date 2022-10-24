#include "VertexBuffer.h"

#include "glad/glad.h"
#include <cstdint>

#include "glm/fwd.hpp"

namespace Polyboid
{
    VertexBuffer::VertexBuffer(uint32_t size): m_BufferSize(size)
    {
        glCreateBuffers(1, &m_ID);
        glBindBuffer(GL_ARRAY_BUFFER, m_ID);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }

    VertexBuffer::VertexBuffer(const void* verts, uint32_t size)
    {
        glCreateBuffers(1, &m_ID);
        glBindBuffer(GL_ARRAY_BUFFER, m_ID);
        glBufferData(GL_ARRAY_BUFFER, size, verts, GL_STATIC_DRAW);
    }

    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers(1, &m_ID);
    }

    void VertexBuffer::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
    {
        m_IndexBuffer = indexBuffer; 
    }

    void VertexBuffer::DescribeBuffer(std::initializer_list<VertexBufferLayout> layouts)
    {
        uint32_t count = 0, stride = 0;

        
        for (auto layout : layouts)
        {
           m_VertexSizeInBytes += layout.CalculateBytes();
            if (count > 0)
            {
                if (!m_Layout.empty())
                {
                    stride += m_Layout[m_Layout.size() - 1].CalculateBytes();
                    layout.CalculateStride(stride);
                }
               
            }

            count++;
            m_Layout.emplace_back(layout);
          
        }
    }

    void VertexBuffer::Bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    }

    void VertexBuffer::SetData(const void* Data, uint32_t size)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_ID);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size == 0 ? m_BufferSize : size, Data);
    }

    std::shared_ptr<VertexBuffer> VertexBuffer::MakeVertexBuffer(const void* verts, uint32_t size)
    {
        return std::make_shared<VertexBuffer>(verts, size);
    }

    std::shared_ptr<VertexBuffer> VertexBuffer::MakeVertexBuffer(uint32_t size)
    {
        return std::make_shared<VertexBuffer>(size);
    }
}
