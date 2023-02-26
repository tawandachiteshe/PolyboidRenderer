#include "boidpch.h"
#include "GLVertexBufferArray.h"

#include "GLIndexBuffer.h"
#include "GLShaderStorageBuffer.h"
#include "GLVertexBuffer.h"
#include "Utils.h"
#include "glad/glad.h"


namespace Polyboid
{

    GLenum GetOpenglShaderDataType(const ShaderDataType& type)
    {
        switch (type)
        {
        case ShaderDataType::None: return 0;
        case ShaderDataType::Float:
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4:
        case ShaderDataType::Mat3:
        case ShaderDataType::Mat4:
            return GL_FLOAT;
        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:
            return GL_INT;
        case ShaderDataType::Bool:
            return  GL_BOOL;
        }
    }

    GLVertexBufferArray::GLVertexBufferArray()
    {
        glCreateVertexArrays(1, &m_Handle);
    }

    bool GLVertexBufferArray::IsUsingVertexPulling()
    {
        return m_IsUsingVertexPulling;
    }

    void GLVertexBufferArray::AddVertexBuffer(const Ref<VertexBuffer>& vbuffer)
    {
        
        const uint32_t vertexBufferCount = m_VertexBuffers.size();
        uint32_t vertexBufferIndex = vertexBufferCount + 1;
        
        auto glvertexBuffer = std::dynamic_pointer_cast<GLVertexbuffer>(vbuffer);

        const auto layout = glvertexBuffer->GetLayout();

        glVertexArrayVertexBuffer(m_Handle, vertexBufferIndex, glvertexBuffer->m_Handle, 0, layout.GetStride());

        //TODO: Make vertex array support more that one vertexbuffer
        
        uint32_t count = 0;
        for (const auto& element : layout)
        {

            glEnableVertexArrayAttrib(m_Handle, count);
            glVertexArrayAttribFormat(m_Handle, count,
                element.GetComponentCount(), GetOpenglShaderDataType(element.Type),
                element.Normalized ? GL_TRUE : GL_FALSE, element.Offset);
            glVertexArrayAttribBinding(m_Handle, count, vertexBufferIndex);
            
            count++;
        }

        m_VertexBuffers.push_back(vbuffer);
        
    }

    void GLVertexBufferArray::SetIndexBuffer(const Ref<IndexBuffer>& iBuffer)
    {
        auto glIndexbuffer = std::dynamic_pointer_cast<GLIndexBuffer>(iBuffer);
        m_IndexBuffer = glIndexbuffer;

        m_Count = iBuffer->GetCount();
        m_IndexDatatype = iBuffer->GetIndexDataType();

        glVertexArrayElementBuffer(m_Handle, glIndexbuffer->m_Handle);
    }

    void GLVertexBufferArray::SetIndexBuffer(const Ref<ShaderStorageBuffer>& buffer, uint32_t count,
        const IndexDataType& type)
    {
        auto glStoragebuffer = std::dynamic_pointer_cast<GLShaderStorageBuffer>(buffer);
        m_IsUsingVertexPulling = true;
        m_Count = count;
        m_IndexDatatype = type;



        glVertexArrayElementBuffer(m_Handle, glStoragebuffer->m_Handle);
    }

    uint32_t GLVertexBufferArray::GetIndexCount()
    {
        return m_Count;
    }

    IndexDataType GLVertexBufferArray::GetIndexDataType()
    {
        return m_IndexDatatype;
    }

    GLVertexBufferArray::~GLVertexBufferArray()
    {
        glDeleteVertexArrays(1, &m_Handle);
    }

    void GLVertexBufferArray::SetIndexCount(uint32_t count)
    {
        m_Count = count;
    }

    void GLVertexBufferArray::Bind()
    {
	    for (auto& vertexBuffer : m_VertexBuffers)
	    {
            vertexBuffer->Bind();
	    }
	    if (m_IndexBuffer)
	    {
            m_IndexBuffer->Bind();
	    }
      

        glBindVertexArray(m_Handle);
    }

    void GLVertexBufferArray::UnBind()
    {
        glBindVertexArray(0);
    }
}
