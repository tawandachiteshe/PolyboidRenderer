#pragma once
#include "Engine/Renderer/Buffer.h"


namespace Polyboid
{
    class GLShaderStorageBuffer : public ShaderStorageBuffer
    {
    private:
        uint32_t m_Handle = 0;
        
    public:

        GLShaderStorageBuffer(uint32_t dataSize);
        GLShaderStorageBuffer(const void* data, uint32_t dataSize);
        
        void Bind(uint32_t slot = 0) const override;
        void Unbind() const override;
        void SetData(const void* data, uint32_t sizeData, uint32_t offset) override;
        ~GLShaderStorageBuffer() override;

        friend class GLVertexBufferArray;
    };
}
