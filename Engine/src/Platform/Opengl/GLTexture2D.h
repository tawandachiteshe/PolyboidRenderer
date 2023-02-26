#pragma once
#include "Engine/Renderer/Texture.h"


namespace Polyboid
{
    class GLTexture2D : public Texture
    {
    private:
        uint32_t m_Handle = 0;
        TextureSettings m_Settings;
        
        
    public:

        GLTexture2D(const TextureSettings& settings);
        GLTexture2D(const void* data, const TextureSettings& settings);
        uint32_t GetHandle() override;

        void SetData(const void* data, uint32_t size) override;
        void Bind(uint32_t slot) override;
        void UnBind() override;
        ~GLTexture2D() override;

        friend class GLFramebuffer;
    };

}

