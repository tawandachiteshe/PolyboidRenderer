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
        uint32_t m_VertexBufferIndex = 0;
    public:
        VertexBufferArray();
        ~VertexBufferArray();
        void CreateVertexBufferLayout();

        const Ref<VertexBuffer>& GetVertexBuffer() { return m_VertexBuffer; }
        const Ref<IndexBuffer>& GetIndexBuffer() { return m_IndexBuffer; }

        const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const { return m_Buffers; }
        
        void AddVertexBuffer(const Ref<VertexBuffer>& buffer);
        void SetIndexBuffer(const Ref<IndexBuffer>& buffer);
        void Bind();
        static std::shared_ptr<VertexBufferArray> MakeVertexBufferArray();
    };

    
}
