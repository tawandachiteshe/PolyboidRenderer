#pragma once
#include <cstdint>
#include <memory>

#include "VertexBuffer.h"
#include "Engine/Core/Base.h"

namespace Polyboid
{

    class VertexBufferArray
    {
    private:
        uint32_t m_ID;
        std::vector<Ref<VertexBuffer>> m_Buffers;
        Ref<VertexBuffer> m_VertexBuffer = nullptr;
        Ref<IndexBuffer> m_IndexBuffer = nullptr;
        
    public:
        VertexBufferArray();
        void CreateVertexBufferLayout();

        const Ref<VertexBuffer>& GetVertexBuffer() { return m_VertexBuffer; }
        const Ref<IndexBuffer>& GetIndexBuffer() { return m_IndexBuffer; }
        
        
        void AddVertexBuffer(const Ref<VertexBuffer>& buffer);
        void SetIndexBuffer(const Ref<IndexBuffer>& buffer);
        void Bind();
        static std::shared_ptr<VertexBufferArray> MakeVertexBufferArray();
    };

    
}
