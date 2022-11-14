
#include "boidpch.h"
#include "VertexBufferArray.h"


#include "Engine/Engine/Debug/Profiler.h"
#include "glad/glad.h"


namespace Polyboid
{
    VertexBufferArray::VertexBufferArray(): m_ID(0)
    {
        POLYBOID_PROFILE_FUNCTION();
        //Generate this before anything
        glCreateVertexArrays(1, &m_ID);
    }

    VertexBufferArray::~VertexBufferArray()
    {
        POLYBOID_PROFILE_FUNCTION();
        glDeleteVertexArrays(1, &m_ID);
    }

    void VertexBufferArray::CreateVertexBufferLayout()
    {
        POLYBOID_PROFILE_FUNCTION();
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    void VertexBufferArray::AddVertexBuffer(const Ref<VertexBuffer>& buffer)
    {
        POLYBOID_PROFILE_FUNCTION();
        m_VertexBuffer = buffer;
        glBindVertexArray(m_ID);
        m_VertexBuffer->Bind();
        
        uint32_t layoutIndex = 0;
        uint32_t vertexSizeInBytes = 0;
        for (auto layout : buffer->GetLayout())
        {
            vertexSizeInBytes += layout.CalculateBytes();
            glEnableVertexAttribArray(layoutIndex);
            glVertexAttribPointer(layoutIndex, (GLint)layout.component, GL_FLOAT, GL_FALSE, buffer->GetVertexSizeInBytes(), reinterpret_cast<void*>(layout.GetStride()));
            layoutIndex++;
            m_VertexBufferIndex++;
        }

        m_Buffers.emplace_back(buffer);
        
    }

    void VertexBufferArray::SetIndexBuffer(const Ref<IndexBuffer>& buffer)
    {
        POLYBOID_PROFILE_FUNCTION();
        m_IndexBuffer = buffer;
        glBindVertexArray(m_ID);
        buffer->Bind();
    }

    void VertexBufferArray::Bind()
    {
        POLYBOID_PROFILE_FUNCTION();
        glBindVertexArray(m_ID);
    }

    std::shared_ptr<VertexBufferArray> VertexBufferArray::MakeVertexBufferArray()
    {
        POLYBOID_PROFILE_FUNCTION();
        return std::make_shared<VertexBufferArray>();
    }
}
