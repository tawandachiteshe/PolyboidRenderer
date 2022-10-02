#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "IndexBuffer.h"
#include "Engine/Core/Base.h"

namespace Polyboid
{
    enum class BufferComponent
    {
        Float, Int = 1,
        Float2, Int2 = 2,
        Float3, Int3 = 3,
        Float4, Int4 = 4,
    };
    
    struct VertexBufferLayout
    {

    private:
        uint32_t m_Stride;
        uint32_t m_Bytes;

        
    public:  
        VertexBufferLayout(const BufferComponent& component, const std::string& name) : m_Stride(0), m_Bytes(0), name(name), component(component) {}

        std::string name;
        BufferComponent component;

    public:
        void CalculateStride(uint32_t stride)
        {
            m_Stride = stride;
        }

        [[nodiscard]]
        uint32_t GetStride() const { return m_Stride; }
        
        uint32_t CalculateBytes()
        {
            m_Bytes = (uint32_t)component * 4;
            return m_Bytes;
        }

        
        
    };
    class VertexBuffer
    {

    private:
        uint32_t m_ID = 0;
        uint32_t m_VertexSizeInBytes = 0;
        std::vector<VertexBufferLayout> m_Layout;
        Ref<IndexBuffer> m_IndexBuffer = nullptr;
  
    
    public:
        VertexBuffer(uint32_t size);
        VertexBuffer(const float *verts, uint32_t size);
        ~VertexBuffer();
        
        uint32_t GetVertexSizeInBytes() { return m_VertexSizeInBytes; }
        

        void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer);
        const Ref<IndexBuffer>& GetIndexBuffer() { return m_IndexBuffer; }
        const std::vector<VertexBufferLayout>& GetLayout() { return m_Layout; }
        
        void DescribeBuffer(std::initializer_list<VertexBufferLayout>  layout);
        void Bind();

        static std::shared_ptr<VertexBuffer> MakeVertexBuffer(const float *verts, uint32_t size);
        static std::shared_ptr<VertexBuffer> MakeVertexBuffer(uint32_t size);
    
    };

}

