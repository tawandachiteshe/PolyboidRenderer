#include "boidpch.h"
#include "Texture2D.h"

#include <stb/stb_image.h>

#include "Engine/Engine/Debug/Profiler.h"
#include "glad/glad.h"


namespace Polyboid
{
    Texture::Texture(const std::string& textureImagePath)
    {

        POLYBOID_PROFILE_FUNCTION();

        stbi_set_flip_vertically_on_load(true);
        uint8_t* data = stbi_load(textureImagePath.c_str(), &m_Width, &m_Height, &m_Channels, 0);

        GLenum internalFormat = 0, dataFormat = 0;
        if (m_Channels == 4)
        {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        }
        else if (m_Channels == 3)
        {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
        }

        m_InternalFormat = internalFormat;
        m_DataFormat = dataFormat;

        glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
        glTextureStorage2D(m_TextureID, 1, internalFormat, m_Width, m_Height);

        glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

        glCreateSamplers(1, &m_SamplersID);
        
        
        stbi_image_free(data);
    }

    Texture::Texture(int32_t width, int32_t height, int32_t channels): m_Width(width), m_Height(height), m_Channels(channels)
    {

        POLYBOID_PROFILE_FUNCTION();

        m_InternalFormat = channels == 4 ? GL_RGBA8 : GL_RGB8;
        m_DataFormat = channels == 4 ? GL_RGBA : GL_RGB;

        glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
        glTextureStorage2D(m_TextureID, 1, m_InternalFormat, m_Width, m_Height);
        glCreateSamplers(1, &m_SamplersID);

        glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    void Texture::SetData(void* data, uint32_t size)
    {
        POLYBOID_PROFILE_FUNCTION();
   
        uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
        glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
    }

    void Texture::Bind(uint32_t slot)
    {
        POLYBOID_PROFILE_FUNCTION();

        glBindTextureUnit(slot, m_TextureID);
        glBindSampler(slot, m_SamplersID);
        
    }

    Texture::~Texture()
    {
        POLYBOID_PROFILE_FUNCTION();

    	glDeleteTextures(1, &m_TextureID);
    }

    Ref<Texture> Texture::MakeTexture2D(const std::string& textureImagePath)
    {
        POLYBOID_PROFILE_FUNCTION();

        return std::make_shared<Texture>(textureImagePath);
    }

    Ref<Texture> Texture::MakeTexture2D(int32_t width, int32_t height, int32_t channels)
    {
        POLYBOID_PROFILE_FUNCTION();

        return std::make_shared<Texture>(width, height, channels);
    }
}
