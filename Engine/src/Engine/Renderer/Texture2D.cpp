#include "boidpch.h"
#include "Texture2D.h"

#include <stb/stb_image.h>

#include "Engine/Engine/Debug/Profiler.h"
#include "glad/glad.h"


namespace Polyboid
{
	Texture::Texture(int32_t width, int32_t height, const TextureSettings& settings): m_Width(width), m_Height(height), m_InternalFormat(0)
	{
		switch (settings.format)
		{
			case TextureInternalFormat::RG16F:
	            m_InternalFormat = GL_RG32F;
	            m_DataFormat = GL_RG;
	            break;
			case TextureInternalFormat::RGBA32F:
	            m_InternalFormat = GL_RGBA32F;
	            m_DataFormat = GL_RGBA;
	            break;
			case TextureInternalFormat::RGBA8:
	            m_InternalFormat = GL_RGBA8;
	            m_DataFormat = GL_RGBA;
	            break;
			case TextureInternalFormat::R32F:
				m_InternalFormat = GL_R32F;
                m_DataFormat = GL_RED;
				break;
            case TextureInternalFormat::R32I:
                m_InternalFormat = GL_R32I;
                m_DataFormat = GL_RED;
                break;
		case TextureInternalFormat::DEPTH:
            m_InternalFormat = GL_UNSIGNED_INT_24_8;
            m_DataFormat = GL_DEPTH_COMPONENT;
            break;
		}


		switch (settings.wrap)
		{
            case TextureWrap::Repeat:
                m_TextureWrap = GL_REPEAT;
                break;
            case TextureWrap::MirroredRepeat:
                m_TextureWrap = GL_MIRRORED_REPEAT;
                break;
            case TextureWrap::ClampToEdge:
                m_TextureWrap = GL_CLAMP_TO_EDGE;
                break;
		}

        glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);

        glTextureStorage2D(m_TextureID, 1, m_InternalFormat, m_Width, m_Height);

        glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, m_TextureWrap);
        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, m_TextureWrap);

	}

	Texture::Texture(int32_t width, int32_t height, int32_t channels, void* data): m_InternalFormat(0), m_Width(width),
		m_Height(height), m_Channels(channels)
	{

        GLenum internalFormat = 0, dataFormat = 0;
        if (m_Channels == 4)
        {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        }
        else if (m_Channels == 3)
        {
            internalFormat =  GL_RGB8;
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

	}

	Texture::Texture(const std::string& textureImagePath, bool isHdri)
    {

        POLYBOID_PROFILE_FUNCTION();

        stbi_set_flip_vertically_on_load(true);
    	uint8_t* data = nullptr;
        float* fdata = nullptr;

        if (isHdri)
        {
            fdata = stbi_loadf(textureImagePath.c_str(), &m_Width, &m_Height, &m_Channels, 0);
        }
        else
        {
            data = stbi_load(textureImagePath.c_str(), &m_Width, &m_Height, &m_Channels, 0);
        }


        GLenum internalFormat = 0, dataFormat = 0;
        if (m_Channels == 4)
        {
            internalFormat = isHdri ? GL_RGBA32F : GL_RGBA8;
            dataFormat = GL_RGBA;
        }
        else if (m_Channels == 3)
        {
            internalFormat = isHdri ? GL_RGB32F : GL_RGB8;
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

        if (isHdri)
        {
            glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_FLOAT , fdata);
        }
        else
        {
            glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);
        }



        if (data)
        {
            stbi_image_free(data);
        }

        if (fdata)
        {
            stbi_image_free(fdata);
        }
        
        
    }

    Texture::Texture(int32_t width, int32_t height, int32_t channels): m_Width(width), m_Height(height), m_Channels(channels)
    {

        POLYBOID_PROFILE_FUNCTION();

        m_InternalFormat = channels == 4 ? GL_RGBA8 : GL_RGB8;
        m_DataFormat = channels == 4 ? GL_RGBA : GL_RGB;

        glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
        glTextureStorage2D(m_TextureID, 1, m_InternalFormat, m_Width, m_Height);


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

    void Texture::Bind(uint32_t slot, bool computeTexture)
    {
        POLYBOID_PROFILE_FUNCTION();

        if (computeTexture)
        {
            glBindImageTexture(0, m_TextureID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
        }
        else
        {
            glBindTextureUnit(slot, m_TextureID);
        }


       
    }

    Texture::~Texture()
    {
        POLYBOID_PROFILE_FUNCTION();

    	glDeleteTextures(1, &m_TextureID);
    }

    Ref<Texture> Texture::MakeTexture2D(const std::string& textureImagePath, bool isHdri)
    {
        POLYBOID_PROFILE_FUNCTION();

        return std::make_shared<Texture>(textureImagePath, isHdri);
    }

    Ref<Texture> Texture::MakeTexture2D(int32_t width, int32_t height, int32_t channels)
    {
        POLYBOID_PROFILE_FUNCTION();

        return std::make_shared<Texture>(width, height, channels);
    }

    Ref<Texture> Texture::MakeTexture2D(int32_t width, int32_t height, int32_t channels, void* data)
    {
        return std::make_shared<Texture>(width, height, channels, data);
    }

    Ref<Texture> Texture::MakeTexture2D(int32_t width, int32_t height, const TextureSettings& settings)
    {
        return std::make_shared<Texture>(width, height, settings);
    }
}
