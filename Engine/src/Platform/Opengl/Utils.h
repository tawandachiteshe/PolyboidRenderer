#pragma once
#include "Engine/Renderer/Texture.h"

typedef unsigned int GLenum;
namespace Polyboid
{
	class Utils
	{
	public:
        static GLenum ConvertOpenGLTextureDataType(TextureDataType dataType);
        static GLenum ConvertTextureFormatToOpenGL(TextureFormat format);
        static int32_t ConvertOpenGLTextureWrapMode(TextureWrapMode wrapMode);
        static int32_t ConvertToOpenGLTextureFilterMode(TextureFilterMode filterMode);
        static GLenum ConvertOpenGLTextureSizedInternalFormat(EngineGraphicsFormats format);
        static GLenum ConvertTextureAccessToOpenGL(TextureAccess access);
        static uint32_t ConvertTextureSizedFormatToBytes(EngineGraphicsFormats format);
        static  uint32_t GetTextureSizedBitCount(EngineGraphicsFormats format);
        static  uint32_t GetPerComponentCount(EngineGraphicsFormats format);
	};

}
