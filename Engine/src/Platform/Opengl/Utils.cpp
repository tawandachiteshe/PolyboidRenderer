#include "boidpch.h"
#include "Utils.h"

#include "Engine/Renderer/Texture.h"
#include "glad/glad.h"


namespace Polyboid
{
	GLenum Utils::ConvertTextureFormatToOpenGL(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::RGBA:
			return GL_RGBA;
		case TextureFormat::RGB:
			return GL_RGB;
		case TextureFormat::RG:
			return GL_RG;
		case TextureFormat::R:
			return GL_RED;
		case TextureFormat::Depth:
			return GL_DEPTH_COMPONENT;
		case TextureFormat::Stencil:
			return GL_STENCIL_INDEX;
		case TextureFormat::DepthStencil:
			return GL_DEPTH_STENCIL;
		default: return 0;
		}
	}

	GLenum Utils::ConvertOpenGLTextureDataType(TextureDataType dataType)
	{
		switch (dataType)
		{
		case TextureDataType::UnsignedByte:
			return GL_UNSIGNED_BYTE;
		case TextureDataType::Byte:
			return GL_BYTE;
		case TextureDataType::UnsignedShort:
			return GL_UNSIGNED_SHORT;
		case TextureDataType::Short:
			return GL_SHORT;
		case TextureDataType::UnsignedInt:
			return GL_UNSIGNED_INT;
		case TextureDataType::Int:
			return GL_INT;
		case TextureDataType::HalfFloat:
			return GL_HALF_FLOAT;
		case TextureDataType::Float:
			return GL_FLOAT;
		case TextureDataType::Double:
			return GL_DOUBLE;
		default:
			return 0;
		}
	}

	int32_t Utils::ConvertOpenGLTextureWrapMode(TextureWrapMode wrapMode)
	{
		switch (wrapMode)
		{
		case TextureWrapMode::Repeat:
			return GL_REPEAT;
		case TextureWrapMode::MirroredRepeat:
			return GL_MIRRORED_REPEAT;
		case TextureWrapMode::ClampToEdge:
			return GL_CLAMP_TO_EDGE;
		case TextureWrapMode::ClampToBorder:
			return GL_CLAMP_TO_BORDER;
		default:
			return GL_REPEAT;
		}
	}


	int32_t Utils::ConvertToOpenGLTextureFilterMode(TextureFilterMode filterMode)
	{
		switch (filterMode)
		{
		case TextureFilterMode::Nearest:
			return GL_NEAREST;
		case TextureFilterMode::Linear:
			return GL_LINEAR;
		case TextureFilterMode::NearestMipmapNearest:
			return GL_NEAREST_MIPMAP_NEAREST;
		case TextureFilterMode::LinearMipmapNearest:
			return GL_LINEAR_MIPMAP_NEAREST;
		case TextureFilterMode::NearestMipmapLinear:
			return GL_NEAREST_MIPMAP_LINEAR;
		case TextureFilterMode::LinearMipmapLinear:
			return GL_LINEAR_MIPMAP_LINEAR;
		default:
			return GL_NEAREST;
		}
	}

	GLenum Utils::ConvertOpenGLTextureSizedInternalFormat(TextureSizedInternalFormat format)
	{
		switch (format)
		{
		case TextureSizedInternalFormat::RGBA8: return GL_RGBA8;
		case TextureSizedInternalFormat::RGBA16F: return GL_RGBA16F;
		case TextureSizedInternalFormat::RGBA32F: return GL_RGBA32F;
		case TextureSizedInternalFormat::RGB8: return GL_RGB8;
		case TextureSizedInternalFormat::RGB16F: return GL_RGB16F;
		case TextureSizedInternalFormat::RGB32F: return GL_RGB32F;
		case TextureSizedInternalFormat::RG8: return GL_RG8;
		case TextureSizedInternalFormat::RG16F: return GL_RG16F;
		case TextureSizedInternalFormat::RG32F: return GL_RG32F;
		case TextureSizedInternalFormat::R8: return GL_R8;
		case TextureSizedInternalFormat::R16F: return GL_R16F;
		case TextureSizedInternalFormat::R32F: return GL_R32F;
		case TextureSizedInternalFormat::DepthComponent16: return GL_DEPTH_COMPONENT16;
		case TextureSizedInternalFormat::DepthComponent24: return GL_DEPTH_COMPONENT24;
		case TextureSizedInternalFormat::DepthComponent32F: return GL_DEPTH_COMPONENT32F;
		case TextureSizedInternalFormat::Depth24Stencil8: return GL_DEPTH24_STENCIL8;
		case TextureSizedInternalFormat::Depth32FStencil8: return GL_DEPTH32F_STENCIL8;

		// Additional formats with different data types
		case TextureSizedInternalFormat::RGBA8UI: return GL_RGBA8UI;
		case TextureSizedInternalFormat::RGBA16UI: return GL_RGBA16UI;
		case TextureSizedInternalFormat::RGBA32UI: return GL_RGBA32UI;
		case TextureSizedInternalFormat::RGB8UI: return GL_RGB8UI;
		case TextureSizedInternalFormat::RGB16UI: return GL_RGB16UI;
		case TextureSizedInternalFormat::RGB32UI: return GL_RGB32UI;
		case TextureSizedInternalFormat::RG8UI: return GL_RG8UI;
		case TextureSizedInternalFormat::RG16UI: return GL_RG16UI;
		case TextureSizedInternalFormat::RG32UI: return GL_RG32UI;
		case TextureSizedInternalFormat::R8UI: return GL_R8UI;
		case TextureSizedInternalFormat::R16UI: return GL_R16UI;
		case TextureSizedInternalFormat::R32UI: return GL_R32UI;
		case TextureSizedInternalFormat::RGBA8I: return GL_RGBA8I;
		case TextureSizedInternalFormat::RGBA16I: return GL_RGBA16I;
		case TextureSizedInternalFormat::RGBA32I: return GL_RGBA32I;
		case TextureSizedInternalFormat::RGB8I: return GL_RGB8I;
		case TextureSizedInternalFormat::RGB16I: return GL_RGB16I;
		case TextureSizedInternalFormat::RGB32I: return GL_RGB32I;
		case TextureSizedInternalFormat::RG8I: return GL_RG8I;
		case TextureSizedInternalFormat::RG16I: return GL_RG16I;
		case TextureSizedInternalFormat::RG32I: return GL_RG32I;
		case TextureSizedInternalFormat::R16I: return GL_R16I;
		case TextureSizedInternalFormat::R32I: return GL_R32I;
		default: return GL_RGBA8;
		}
	}

	GLenum ConvertTextureAccessToOpenGL(TextureAccess access)
	{
		switch (access)
		{
		case TextureAccess::Read: return GL_READ_ONLY;
		case TextureAccess::Write: return GL_WRITE_ONLY;
		case TextureAccess::ReadWrite: return GL_READ_WRITE;
		}
	}

	uint32_t Utils::ConvertTextureSizedFormatToBytes(TextureSizedInternalFormat format)
	{
		return Utils::GetTextureSizedBitCount(format) / 8;
	}

	uint32_t Utils::GetTextureSizedBitCount(TextureSizedInternalFormat format)
	{
		switch (format)
		{
		case TextureSizedInternalFormat::R8:
		case TextureSizedInternalFormat::R8UI:
		case TextureSizedInternalFormat::R8I:
			return 8;
		case TextureSizedInternalFormat::R16F:
		case TextureSizedInternalFormat::R16UI:
		case TextureSizedInternalFormat::R16I:
		case TextureSizedInternalFormat::RG8:
		case TextureSizedInternalFormat::RG8UI:
		case TextureSizedInternalFormat::RG8I:
		case TextureSizedInternalFormat::DepthComponent16:
			return 16;
		case TextureSizedInternalFormat::RGB8:
		case TextureSizedInternalFormat::RGB8UI:
		case TextureSizedInternalFormat::RGB8I:
			return 24;
		case TextureSizedInternalFormat::R32F:
		case TextureSizedInternalFormat::R32UI:
		case TextureSizedInternalFormat::R32I:
		case TextureSizedInternalFormat::RG16F:
		case TextureSizedInternalFormat::RG16UI:
		case TextureSizedInternalFormat::RG16I:
		case TextureSizedInternalFormat::RGBA8:
		case TextureSizedInternalFormat::RGBA8UI:
		case TextureSizedInternalFormat::RGBA8I:
		case TextureSizedInternalFormat::DepthComponent32F:
		case TextureSizedInternalFormat::Depth24Stencil8:
			return 32;
		case TextureSizedInternalFormat::Depth32FStencil8:
			return 40;
		case TextureSizedInternalFormat::RGB16F:
		case TextureSizedInternalFormat::RGB16UI:
		case TextureSizedInternalFormat::RGB16I:
			return 48;
		case TextureSizedInternalFormat::RG32F:
		case TextureSizedInternalFormat::RG32UI:
		case TextureSizedInternalFormat::RG32I:
		case TextureSizedInternalFormat::RGBA16F:
		case TextureSizedInternalFormat::RGBA16UI:
		case TextureSizedInternalFormat::RGBA16I:
			return 64;
		case TextureSizedInternalFormat::RGB32F:
		case TextureSizedInternalFormat::RGB32UI:
		case TextureSizedInternalFormat::RGB32I:
			return 96;
		case TextureSizedInternalFormat::RGBA32F:
		case TextureSizedInternalFormat::RGBA32UI:
		case TextureSizedInternalFormat::RGBA32I:
			return 128;
		default:
			return -1; // return -1 for unsupported or invalid format
		}
	}

	uint32_t Utils::GetPerComponentCount(TextureSizedInternalFormat format)
	{
		switch (format)
		{
		case TextureSizedInternalFormat::R8:
		case TextureSizedInternalFormat::R8UI:
		case TextureSizedInternalFormat::R8I:
		case TextureSizedInternalFormat::R16F:
		case TextureSizedInternalFormat::R16UI:
		case TextureSizedInternalFormat::R16I:
		case TextureSizedInternalFormat::R32F:
		case TextureSizedInternalFormat::R32UI:
		case TextureSizedInternalFormat::R32I:
			return 1;
		case TextureSizedInternalFormat::RG8:
		case TextureSizedInternalFormat::RG8UI:
		case TextureSizedInternalFormat::RG8I:
		case TextureSizedInternalFormat::RG16F:
		case TextureSizedInternalFormat::RG16UI:
		case TextureSizedInternalFormat::RG16I:
		case TextureSizedInternalFormat::RG32F:
		case TextureSizedInternalFormat::RG32UI:
		case TextureSizedInternalFormat::RG32I:
			return 2;
		case TextureSizedInternalFormat::RGB8:
		case TextureSizedInternalFormat::RGB8UI:
		case TextureSizedInternalFormat::RGB8I:
		case TextureSizedInternalFormat::RGB16F:
		case TextureSizedInternalFormat::RGB16UI:
		case TextureSizedInternalFormat::RGB16I:
		case TextureSizedInternalFormat::RGB32F:
		case TextureSizedInternalFormat::RGB32UI:
		case TextureSizedInternalFormat::RGB32I:
			return 3;
		case TextureSizedInternalFormat::RGBA8:
		case TextureSizedInternalFormat::RGBA8UI:
		case TextureSizedInternalFormat::RGBA8I:
		case TextureSizedInternalFormat::RGBA16F:
		case TextureSizedInternalFormat::RGBA16UI:
		case TextureSizedInternalFormat::RGBA16I:
		case TextureSizedInternalFormat::RGBA32F:
		case TextureSizedInternalFormat::RGBA32UI:
		case TextureSizedInternalFormat::RGBA32I:
			return 4;
		case TextureSizedInternalFormat::DepthComponent16:
		case TextureSizedInternalFormat::DepthComponent32F:
			return 1;
		case TextureSizedInternalFormat::Depth24Stencil8:
		case TextureSizedInternalFormat::Depth32FStencil8:
			return 2;
		default:
			return -1; // return -1 for unsupported or invalid format
		}
	}
}
