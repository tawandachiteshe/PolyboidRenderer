#pragma once
#include <cstdint>
#include <string>
#include <glad/glad.h>

#include "Engine/Engine/Base.h"

namespace Polyboid
{
    enum TextureSettings
    {
        Repeat,
        MirroredRepeat
    };

    
    class Texture
    {
    private:

        uint32_t m_TextureID = 0;
        uint32_t m_SamplersID = 0;
        GLenum m_InternalFormat, m_DataFormat = 0;
        int32_t m_Width = 0, m_Height = 0, m_Channels = 0;
        
    public:
        Texture(const std::string& textureImagePath);
        Texture(uint32_t width, uint32_t height);
        void SetData(void* data, uint32_t size);
        void Bind(uint32_t slot = 0);

        uint32_t GetTextureID() const { return m_TextureID; }
        


        ~Texture();

        static Ref<Texture> MakeTexture2D(const std::string& textureImagePath);
        static Ref<Texture> MakeTexture2D(uint32_t width, uint32_t height);
        
    };

}
