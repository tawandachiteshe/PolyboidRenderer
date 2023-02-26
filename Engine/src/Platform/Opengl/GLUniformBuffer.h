#pragma once
#include "Engine/Renderer/UniformBuffer.h"


namespace Polyboid
{
    class GLUniformBuffer : public UniformBuffer
    {
    private:
        uint32_t m_Handle = 0;

        uint32_t m_Slot = 0;
        uint32_t m_DataSize = 0;
        
    public:
        GLUniformBuffer(uint32_t size, uint32_t binding);

        uint32_t GetBindingSlot() override;
        uint32_t GetDataSize() override;

        void SetData(const void* data, uint32_t size, uint32_t offset) override;
        ~GLUniformBuffer() override;
    };
    
}
