#pragma once


#include "VertexBuffer.h"
#include "Engine/Engine/Base.h"

namespace Polyboid
{
	class ShaderBufferStorage;

	class VertexBufferArray
    {
    private:
        uint32_t m_ID;
        std::vector<Ref<VertexBuffer>> m_Buffers;
        Ref<VertexBuffer> m_VertexBuffer = nullptr;
        Ref<IndexBuffer> m_IndexBuffer = nullptr;
        uint32_t m_VertexBufferIndex = 0;
        uint32_t m_IndiceCount = 0;

        Ref<ShaderBufferStorage> m_Storage = nullptr;

    public:
        VertexBufferArray();
        VertexBufferArray(const Ref<ShaderBufferStorage>& indices, uint32_t indiceCount);
        ~VertexBufferArray();
        void CreateVertexBufferLayout();

        uint32_t GetIndicesCount() { return m_IndiceCount; }

        const Ref<VertexBuffer>& GetVertexBuffer() { return m_VertexBuffer; }
        const Ref<IndexBuffer>& GetIndexBuffer() { return m_IndexBuffer; }

        const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const { return m_Buffers; }
        
        void AddVertexBuffer(const Ref<VertexBuffer>& buffer);
        void SetIndexBuffer(const Ref<IndexBuffer>& buffer);
        void SetShaderBufferStorage(const Ref<ShaderBufferStorage>& verts);
        void Bind();
        static std::shared_ptr<VertexBufferArray> MakeVertexBufferArray();
        static std::shared_ptr<VertexBufferArray> MakeVertexBufferArray(const Ref<ShaderBufferStorage>& indices, uint32_t indiceCount);
    };

    
}
