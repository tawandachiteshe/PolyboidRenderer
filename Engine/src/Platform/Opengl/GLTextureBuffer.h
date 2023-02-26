#pragma once
#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/Texture.h"


namespace Polyboid
{
    class GLTextureBuffer : TextureBuffer
    {
    private:
        uint32_t m_Handle = 0;
        uint32_t m_SSBOHandle = 0;
        TextureBufferSpecification m_Settings;
    public:

        GLTextureBuffer(const TextureBufferSpecification& settings);
        
        void Bind(uint32_t slot, uint32_t bufferSlot = 0) const override;
        void Unbind() const override;
        ~GLTextureBuffer() override;
    };

}

