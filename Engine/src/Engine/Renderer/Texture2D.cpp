#include "Texture2D.h"

#include <stb/stb_image.h>

#include "glad/glad.h"


namespace Polyboid
{
    Texture::Texture(const std::string& textureImagePath)
    {
        
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
        
        
        stbi_image_free(data);
    }

    Texture::Texture(uint32_t width, uint32_t height): m_Width(width), m_Height(height)
    {
        m_InternalFormat = GL_RGBA8;
        m_DataFormat = GL_RGBA;

        glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
        glTextureStorage2D(m_TextureID, 1, m_InternalFormat, m_Width, m_Height);

        glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    void Texture::SetData(void* data, uint32_t size)
    {
        uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
        glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
    }

    void Texture::Bind(uint32_t slot)
    {
        glBindTextureUnit(slot, m_TextureID);
    }

    Texture::~Texture()
    {
    }

    Ref<Texture> Texture::MakeTexture2D(const std::string& textureImagePath)
    {
        return std::make_shared<Texture>(textureImagePath);
    }

    Ref<Texture> Texture::MakeTexture2D(uint32_t width, uint32_t height)
    {
        return std::make_shared<Texture>(width, height);
    }
}
