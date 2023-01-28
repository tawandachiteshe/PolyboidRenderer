#pragma once
#include <cstdint>
#include <string>
#include <glad/glad.h>

#include "Engine/Engine/Base.h"

namespace Polyboid
{
	enum class TextureInternalFormat
	{
		RGBA8,
        RGBA32F,
        RG16F,
        R32F,
        R32I,
        DEPTH
	};

    enum TextureWrap
    {
        ClampToEdge,
        Repeat,
        MirroredRepeat
    };

    struct TextureSettings
    {
        TextureInternalFormat format;
        TextureWrap wrap = TextureWrap::Repeat;
    };

    
    class Texture
    {
    private:

        uint32_t m_TextureID = 0;
        GLenum m_InternalFormat, m_DataFormat = 0;
        GLint m_TextureWrap = 0;
        int32_t m_Width = 0, m_Height = 0, m_Channels = 0;
        
    public:
        Texture(int32_t width, int32_t height, const TextureSettings& settings);
        Texture(int32_t width, int32_t height, int32_t channels, void* data);
        Texture(const std::string& textureImagePath, bool isHdri = false);
        Texture(int32_t width, int32_t height, int32_t channels);
        void SetData(void* data, uint32_t size);
        void Bind(uint32_t slot = 0, bool computeTexture = false);
        int32_t GetWidth() const { return m_Width; }
        int32_t GetHeight() const {
            return m_Height;
        }


        uint32_t GetTextureID() const { return m_TextureID; }
        


        ~Texture();

        static Ref<Texture> MakeTexture2D(const std::string& textureImagePath, bool isHdri = false);
        static Ref<Texture> MakeTexture2D(int32_t width, int32_t height, int32_t channels);
        static Ref<Texture> MakeTexture2D(int32_t width, int32_t height, int32_t channels, void* data);
        static Ref<Texture> MakeTexture2D(int32_t width, int32_t height, const TextureSettings& settings);
        
    };

}
