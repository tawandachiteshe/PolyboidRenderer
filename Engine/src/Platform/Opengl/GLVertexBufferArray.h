#pragma once
#include "Engine/Renderer/VertexBufferArray.h"
#include <vector>

namespace Polyboid
{
	class GLIndexBuffer;

	class GLVertexBufferArray : public VertexBufferArray
    {

    private:
        bool m_IsUsingVertexPulling = false;
        uint32_t m_Count = 0;
        IndexDataType m_IndexDatatype = IndexDataType::UnsignedInt;
        uint32_t m_Handle = 0;
        std::vector<Ref<VertexBuffer>> m_VertexBuffers;
        Ref<GLIndexBuffer> m_IndexBuffer;
        
    public:

        GLVertexBufferArray();
        
        bool IsUsingVertexPulling() override;
        void AddVertexBuffer(const Ref<VertexBuffer>& vbuffer) override;
        void SetIndexBuffer(const Ref<IndexBuffer>& iBuffer) override;
        void SetIndexBuffer(const Ref<ShaderStorageBuffer>& buffer, uint32_t count, const IndexDataType& type = IndexDataType::UnsignedInt) override;
        uint32_t GetIndexCount() override;
        IndexDataType GetIndexDataType() override;
        ~GLVertexBufferArray() override;

        void SetIndexCount(uint32_t count) override;

        void Bind() override;
        void UnBind() override;
    };
    
}
