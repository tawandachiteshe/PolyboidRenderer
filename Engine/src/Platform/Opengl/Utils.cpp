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

	GLenum Utils::ConvertOpenGLTextureSizedInternalFormat(EngineGraphicsFormats format)
	{
		switch (format)
		{
		case EngineGraphicsFormats::RGBA8: return GL_RGBA8;
		case EngineGraphicsFormats::RGBA16F: return GL_RGBA16F;
		case EngineGraphicsFormats::RGBA32F: return GL_RGBA32F;
		case EngineGraphicsFormats::RGB8: return GL_RGB8;
		case EngineGraphicsFormats::RGB16F: return GL_RGB16F;
		case EngineGraphicsFormats::RGB32F: return GL_RGB32F;
		case EngineGraphicsFormats::RG8: return GL_RG8;
		case EngineGraphicsFormats::RG16F: return GL_RG16F;
		case EngineGraphicsFormats::RG32F: return GL_RG32F;
		case EngineGraphicsFormats::R8: return GL_R8;
		case EngineGraphicsFormats::R16F: return GL_R16F;
		case EngineGraphicsFormats::R32F: return GL_R32F;
		case EngineGraphicsFormats::DepthComponent16: return GL_DEPTH_COMPONENT16;
		case EngineGraphicsFormats::DepthComponent24: return GL_DEPTH_COMPONENT24;
		case EngineGraphicsFormats::DepthComponent32F: return GL_DEPTH_COMPONENT32F;
		case EngineGraphicsFormats::Depth24Stencil8: return GL_DEPTH24_STENCIL8;
		case EngineGraphicsFormats::Depth32FStencil8: return GL_DEPTH32F_STENCIL8;

		// Additional formats with different data types
		case EngineGraphicsFormats::RGBA8UI: return GL_RGBA8UI;
		case EngineGraphicsFormats::RGBA16UI: return GL_RGBA16UI;
		case EngineGraphicsFormats::RGBA32UI: return GL_RGBA32UI;
		case EngineGraphicsFormats::RGB8UI: return GL_RGB8UI;
		case EngineGraphicsFormats::RGB16UI: return GL_RGB16UI;
		case EngineGraphicsFormats::RGB32UI: return GL_RGB32UI;
		case EngineGraphicsFormats::RG8UI: return GL_RG8UI;
		case EngineGraphicsFormats::RG16UI: return GL_RG16UI;
		case EngineGraphicsFormats::RG32UI: return GL_RG32UI;
		case EngineGraphicsFormats::R8UI: return GL_R8UI;
		case EngineGraphicsFormats::R16UI: return GL_R16UI;
		case EngineGraphicsFormats::R32UI: return GL_R32UI;
		case EngineGraphicsFormats::RGBA8I: return GL_RGBA8I;
		case EngineGraphicsFormats::RGBA16I: return GL_RGBA16I;
		case EngineGraphicsFormats::RGBA32I: return GL_RGBA32I;
		case EngineGraphicsFormats::RGB8I: return GL_RGB8I;
		case EngineGraphicsFormats::RGB16I: return GL_RGB16I;
		case EngineGraphicsFormats::RGB32I: return GL_RGB32I;
		case EngineGraphicsFormats::RG8I: return GL_RG8I;
		case EngineGraphicsFormats::RG16I: return GL_RG16I;
		case EngineGraphicsFormats::RG32I: return GL_RG32I;
		case EngineGraphicsFormats::R16I: return GL_R16I;
		case EngineGraphicsFormats::R32I: return GL_R32I;
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

	uint32_t Utils::ConvertTextureSizedFormatToBytes(EngineGraphicsFormats format)
	{
		return Utils::GetTextureSizedBitCount(format) / 8;
	}

	uint32_t Utils::GetTextureSizedBitCount(EngineGraphicsFormats format)
	{
		switch (format)
		{
		case EngineGraphicsFormats::R8:
		case EngineGraphicsFormats::R8UI:
		case EngineGraphicsFormats::R8I:
			return 8;
		case EngineGraphicsFormats::R16F:
		case EngineGraphicsFormats::R16UI:
		case EngineGraphicsFormats::R16I:
		case EngineGraphicsFormats::RG8:
		case EngineGraphicsFormats::RG8UI:
		case EngineGraphicsFormats::RG8I:
		case EngineGraphicsFormats::DepthComponent16:
			return 16;
		case EngineGraphicsFormats::RGB8:
		case EngineGraphicsFormats::RGB8UI:
		case EngineGraphicsFormats::RGB8I:
			return 24;
		case EngineGraphicsFormats::R32F:
		case EngineGraphicsFormats::R32UI:
		case EngineGraphicsFormats::R32I:
		case EngineGraphicsFormats::RG16F:
		case EngineGraphicsFormats::RG16UI:
		case EngineGraphicsFormats::RG16I:
		case EngineGraphicsFormats::RGBA8:
		case EngineGraphicsFormats::RGBA8UI:
		case EngineGraphicsFormats::RGBA8I:
		case EngineGraphicsFormats::DepthComponent32F:
		case EngineGraphicsFormats::Depth24Stencil8:
			return 32;
		case EngineGraphicsFormats::Depth32FStencil8:
			return 40;
		case EngineGraphicsFormats::RGB16F:
		case EngineGraphicsFormats::RGB16UI:
		case EngineGraphicsFormats::RGB16I:
			return 48;
		case EngineGraphicsFormats::RG32F:
		case EngineGraphicsFormats::RG32UI:
		case EngineGraphicsFormats::RG32I:
		case EngineGraphicsFormats::RGBA16F:
		case EngineGraphicsFormats::RGBA16UI:
		case EngineGraphicsFormats::RGBA16I:
			return 64;
		case EngineGraphicsFormats::RGB32F:
		case EngineGraphicsFormats::RGB32UI:
		case EngineGraphicsFormats::RGB32I:
			return 96;
		case EngineGraphicsFormats::RGBA32F:
		case EngineGraphicsFormats::RGBA32UI:
		case EngineGraphicsFormats::RGBA32I:
			return 128;
		default:
			return -1; // return -1 for unsupported or invalid format
		}
	}

	uint32_t Utils::GetPerComponentCount(EngineGraphicsFormats format)
	{
		switch (format)
		{
		case EngineGraphicsFormats::R8:
		case EngineGraphicsFormats::R8UI:
		case EngineGraphicsFormats::R8I:
		case EngineGraphicsFormats::R16F:
		case EngineGraphicsFormats::R16UI:
		case EngineGraphicsFormats::R16I:
		case EngineGraphicsFormats::R32F:
		case EngineGraphicsFormats::R32UI:
		case EngineGraphicsFormats::R32I:
			return 1;
		case EngineGraphicsFormats::RG8:
		case EngineGraphicsFormats::RG8UI:
		case EngineGraphicsFormats::RG8I:
		case EngineGraphicsFormats::RG16F:
		case EngineGraphicsFormats::RG16UI:
		case EngineGraphicsFormats::RG16I:
		case EngineGraphicsFormats::RG32F:
		case EngineGraphicsFormats::RG32UI:
		case EngineGraphicsFormats::RG32I:
			return 2;
		case EngineGraphicsFormats::RGB8:
		case EngineGraphicsFormats::RGB8UI:
		case EngineGraphicsFormats::RGB8I:
		case EngineGraphicsFormats::RGB16F:
		case EngineGraphicsFormats::RGB16UI:
		case EngineGraphicsFormats::RGB16I:
		case EngineGraphicsFormats::RGB32F:
		case EngineGraphicsFormats::RGB32UI:
		case EngineGraphicsFormats::RGB32I:
			return 3;
		case EngineGraphicsFormats::RGBA8:
		case EngineGraphicsFormats::RGBA8UI:
		case EngineGraphicsFormats::RGBA8I:
		case EngineGraphicsFormats::RGBA16F:
		case EngineGraphicsFormats::RGBA16UI:
		case EngineGraphicsFormats::RGBA16I:
		case EngineGraphicsFormats::RGBA32F:
		case EngineGraphicsFormats::RGBA32UI:
		case EngineGraphicsFormats::RGBA32I:
			return 4;
		case EngineGraphicsFormats::DepthComponent16:
		case EngineGraphicsFormats::DepthComponent32F:
			return 1;
		case EngineGraphicsFormats::Depth24Stencil8:
		case EngineGraphicsFormats::Depth32FStencil8:
			return 2;
		default:
			return -1; // return -1 for unsupported or invalid format
		}
	}
}
