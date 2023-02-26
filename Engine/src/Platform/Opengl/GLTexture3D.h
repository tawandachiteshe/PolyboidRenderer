#pragma once
#include "Engine/Renderer/Texture.h"


namespace Polyboid
{
    class GLTexture3D : public Texture3D
    {
    private:
        TextureSettings m_Settings;
        uint32_t m_Handle = 0;
    public:

        GLTexture3D(const void** data, const TextureSettings& settings);
        
        void Bind(uint32_t slot) override;
        void UnBind() override;
        void SetData(const void** data, uint32_t size) override;
        ~GLTexture3D() override;
    };
    
}
