#pragma once
#include <variant>

#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/RenderAPI.h"


namespace Polyboid
{
    
    class GLIndexBuffer : public IndexBuffer
    {
    private:
        IndexDataType m_DataType = IndexDataType::UnsignedInt;
        uint32_t m_Count = 0;
        uint32_t m_Handle = 0;
        
    public:
        GLIndexBuffer(const IndexDataType& type, uint32_t count, const std::variant<uint32_t*, uint16_t*>& data);
        
        ~GLIndexBuffer() override;
        void Bind() const override;
        void Unbind() const override;
        uint32_t GetCount() const override;
        IndexDataType GetIndexDataType() override;

        friend class GLVertexBufferArray;
        
    };
}
