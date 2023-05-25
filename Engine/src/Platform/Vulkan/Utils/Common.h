#pragma once

#include "Engine/Renderer/Image2D.h"
#include "Engine/Renderer/Texture.h"
#include "vulkan/vulkan.hpp"
#include "spdlog/spdlog.h"


#define CheckResult(result, message) \
	result != vk::Result::eSuccess ? __debugbreak() : spdlog::info(message);


namespace Polyboid
{
	namespace Utils
	{
		inline static vk::ImageAspectFlagBits ImageFormatToAspectBits(EngineGraphicsFormats format)
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
			case EngineGraphicsFormats::RG8:
			case EngineGraphicsFormats::RG8UI:
			case EngineGraphicsFormats::RG8I:
			case EngineGraphicsFormats::RG16F:
			case EngineGraphicsFormats::RG16UI:
			case EngineGraphicsFormats::RG16I:
			case EngineGraphicsFormats::RG32F:
			case EngineGraphicsFormats::RG32UI:
			case EngineGraphicsFormats::RG32I:
			case EngineGraphicsFormats::RGB8:
			case EngineGraphicsFormats::RGB8UI:
			case EngineGraphicsFormats::RGB8I:
			case EngineGraphicsFormats::RGB16F:
			case EngineGraphicsFormats::RGB16UI:
			case EngineGraphicsFormats::RGB16I:
			case EngineGraphicsFormats::RGB32F:
			case EngineGraphicsFormats::RGB32UI:
			case EngineGraphicsFormats::RGB32I:
			case EngineGraphicsFormats::RGBA8:
			case EngineGraphicsFormats::BGRA8:
			case EngineGraphicsFormats::RGBA8UI:
			case EngineGraphicsFormats::RGBA8I:
			case EngineGraphicsFormats::BGRA8I:
			case EngineGraphicsFormats::BGRA8ISrgb:
			case EngineGraphicsFormats::BGRA8U:
			case EngineGraphicsFormats::RGBA16F:
			case EngineGraphicsFormats::RGBA16UI:
			case EngineGraphicsFormats::RGBA16I:
			case EngineGraphicsFormats::RGBA32F:
			case EngineGraphicsFormats::RGBA32UI:
			case EngineGraphicsFormats::RGBA32I:
				return vk::ImageAspectFlagBits::eColor;
			case EngineGraphicsFormats::DepthComponent16:
			case EngineGraphicsFormats::DepthComponent32F:
			case EngineGraphicsFormats::DepthComponent24:
			case EngineGraphicsFormats::Depth24Stencil8:
			case EngineGraphicsFormats::Depth32FStencil8:
				return vk::ImageAspectFlagBits::eDepth;
			}
			return vk::ImageAspectFlagBits::eNone;
		}

		inline static vk::ImageLayout ConvertToVulkanLayout(ImageLayout layout)
		{
			switch (layout)
			{
			case ImageLayout::ColorAttachmentOptimal: return vk::ImageLayout::eColorAttachmentOptimal;
			case ImageLayout::TransferDstOptimal: return vk::ImageLayout::eTransferDstOptimal;
			case ImageLayout::DepthAttachmentOptimal: return vk::ImageLayout::eDepthAttachmentOptimal;
			case ImageLayout::Undefined: return vk::ImageLayout::eUndefined;
			case ImageLayout::ShaderReadOptimal: return vk::ImageLayout::eShaderReadOnlyOptimal;
			}

			__debugbreak();
			return {};
		}

		inline static vk::Format ConvertToVulkanFormat(EngineGraphicsFormats format)
		{
			switch (format)
			{
			case EngineGraphicsFormats::R8:
				return vk::Format::eR8Unorm;
			case EngineGraphicsFormats::R8UI:
				return vk::Format::eR8Uint;
			case EngineGraphicsFormats::R8I:
				return vk::Format::eR8Sint;
			case EngineGraphicsFormats::R16F:
				return vk::Format::eR16Sfloat;
			case EngineGraphicsFormats::R16UI:
				return vk::Format::eR16Uint;
			case EngineGraphicsFormats::R16I:
				return vk::Format::eR16Sint;
			case EngineGraphicsFormats::R32F:
				return vk::Format::eR32Sfloat;
			case EngineGraphicsFormats::R32UI:
				return vk::Format::eR32Uint;
			case EngineGraphicsFormats::R32I:
				return vk::Format::eR32Sint;
			case EngineGraphicsFormats::RG8:
				return vk::Format::eR8G8Unorm;
			case EngineGraphicsFormats::RG8UI:
				return vk::Format::eR8G8Uint;
			case EngineGraphicsFormats::RG8I:
				return vk::Format::eR8G8Sint;
			case EngineGraphicsFormats::RG16F:
				return vk::Format::eR16G16Sfloat;
			case EngineGraphicsFormats::RG16UI:
				return vk::Format::eR16G16Uint;
			case EngineGraphicsFormats::RG16I:
				return vk::Format::eR16G16Sint;
			case EngineGraphicsFormats::RG32F:
				return vk::Format::eR32G32Sfloat;
			case EngineGraphicsFormats::RG32UI:
				return vk::Format::eR32G32Uint;
			case EngineGraphicsFormats::RG32I:
				return vk::Format::eR32G32Sint;
			case EngineGraphicsFormats::RGB8:
				return vk::Format::eR8G8B8Unorm;
			case EngineGraphicsFormats::RGB8UI:
				return vk::Format::eR8G8B8Uint;
			case EngineGraphicsFormats::RGB8I:
				return vk::Format::eR8G8B8Sint;
			case EngineGraphicsFormats::RGB16F:
				return vk::Format::eR16G16B16Sfloat;
			case EngineGraphicsFormats::RGB16UI:
				return vk::Format::eR16G16B16Uint;
			case EngineGraphicsFormats::RGB16I:
				return vk::Format::eR16G16B16Sint;
			case EngineGraphicsFormats::RGB32F:
				return vk::Format::eR32G32B32Sfloat;
			case EngineGraphicsFormats::RGB32UI:
				return vk::Format::eR32G32B32Uint;
			case EngineGraphicsFormats::RGB32I:
				return vk::Format::eR32G32B32Sint;
			case EngineGraphicsFormats::RGBA8:
				return vk::Format::eR8G8B8A8Unorm;
			case EngineGraphicsFormats::RGBA8UI:
				return vk::Format::eR8G8B8A8Uint;
			case EngineGraphicsFormats::RGBA8I:
				return vk::Format::eR8G8B8A8Sint;
			case EngineGraphicsFormats::BGRA8ISrgb:
				return vk::Format::eB8G8R8A8Srgb;
			case EngineGraphicsFormats::RGBA16F:
				return vk::Format::eR16G16B16A16Sfloat;
			case EngineGraphicsFormats::RGBA16UI:
				return vk::Format::eR16G16B16A16Uint;
			case EngineGraphicsFormats::RGBA16I:
				return vk::Format::eR16G16B16A16Sint;
			case EngineGraphicsFormats::RGBA32F:
				return vk::Format::eR32G32B32A32Sfloat;
			case EngineGraphicsFormats::RGBA32UI:
				return vk::Format::eR32G32B32A32Uint;
			case EngineGraphicsFormats::RGBA32I:
				return vk::Format::eR32G32B32A32Sint;
			case EngineGraphicsFormats::DepthComponent16:
				return vk::Format::eD16Unorm;
			case EngineGraphicsFormats::DepthComponent32F:
				return vk::Format::eD32Sfloat;
			case EngineGraphicsFormats::DepthComponent24:
				return vk::Format::eX8D24UnormPack32;
			case EngineGraphicsFormats::Depth24Stencil8:
				return vk::Format::eD24UnormS8Uint;
			case EngineGraphicsFormats::Depth32FStencil8:
				return vk::Format::eD32SfloatS8Uint;

			case EngineGraphicsFormats::BGRA8: return vk::Format::eB8G8R8A8Snorm;
			case EngineGraphicsFormats::BGRA8I: return vk::Format::eB8G8R8A8Sint;
			case EngineGraphicsFormats::BGRA8U: return vk::Format::eB8G8R8A8Unorm;
			default:
				throw std::runtime_error("Invalid format");
			}
		}
	}
}
