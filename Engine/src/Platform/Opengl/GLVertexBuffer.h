#pragma once
#include "Engine/Renderer/Buffer.h"

namespace Polyboid
{
    class GLVertexbuffer : public VertexBuffer
    {

    private:
        uint32_t m_Handle = 0;
        BufferLayout m_Layout;
        
    public:

        //TODO: Make this safe
        GLVertexbuffer(const void* data, uint32_t dataSize);
        GLVertexbuffer(uint32_t dataSize);
        
        ~GLVertexbuffer() override;
        void Bind() const override;

        void Unbind() const override;
        void SetData(const void* data, uint32_t size) override;

        const BufferLayout& GetLayout() const override;
        void SetLayout(const BufferLayout& layout) override;
        friend class GLVertexBufferArray;
        
    };
    
}
