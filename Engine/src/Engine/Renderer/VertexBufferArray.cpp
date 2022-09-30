#include "VertexBufferArray.h"

#include "glad/glad.h"
#include "glm/fwd.hpp"


namespace Polyboid
{
    VertexBufferArray::VertexBufferArray(): m_ID(0)
    {
        //Generate this before anything
        glGenVertexArrays(1, &m_ID);
    }

    void VertexBufferArray::CreateVertexBufferLayout()
    {
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    void VertexBufferArray::AddVertexBuffer(const Ref<VertexBuffer>& buffer)
    {
        m_VertexBuffer = buffer;
        glBindVertexArray(m_ID);
        m_VertexBuffer->Bind();
        
        uint32_t layoutIndex = 0;
        uint32_t vertexSizeInBytes = 0;
        for (auto layout : buffer->GetLayout())
        {
            vertexSizeInBytes += layout.CalculateBytes();
            glEnableVertexAttribArray(layoutIndex);
            glVertexAttribPointer(layoutIndex, (GLint)layout.component, GL_FLOAT, GL_FALSE, buffer->GetVertexSizeInBytes(), (void*)layout.GetStride());
            layoutIndex++;
        }
      
        
    }

    void VertexBufferArray::SetIndexBuffer(const Ref<IndexBuffer>& buffer)
    {
        m_IndexBuffer = buffer;
        glBindVertexArray(m_ID);
        buffer->Bind();
    }

    void VertexBufferArray::Bind()
    {
        glBindVertexArray(m_ID);
    }

    std::shared_ptr<VertexBufferArray> VertexBufferArray::MakeVertexBufferArray()
    {
        return std::make_shared<VertexBufferArray>();
    }
}
