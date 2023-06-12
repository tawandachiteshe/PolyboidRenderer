#pragma once
#include <any>
#include <vulkan/vulkan.hpp>

#include "RenderResource.h"
#include "Engine/Engine/Base.h"


namespace Polyboid
{
	enum class EngineGraphicsFormats;

	enum class ImageLayout
	{
		ColorAttachmentOptimal,
		TransferDstOptimal,
		TransferSrcOptimal,
		DepthAttachmentOptimal,
		ShaderReadOptimal,
		Undefined,
		General
	};

	enum class ImageUsage
	{
		Swapchain,
		ColorAttachment,
		DepthStencilAttachment,
		ColorDepthStencilAttachment,
		Sampling,
		TransferSrc,
		TransferDst,
		ColorAttachmentSampling,
		DepthStencilAttachmentSampling,
		StorageImage
	};

	struct ImageSettings
	{
		ImageUsage usage = ImageUsage::Sampling;
		uint32_t height = 1;
		uint32_t width = 1;
		uint32_t depth = 1;
		uint32_t sampleCount = 1;
		uint32_t mipCount = 1;
		uint32_t layerCount = 1;
		bool generateMips = false;
		EngineGraphicsFormats format;
		ImageLayout layout = ImageLayout::Undefined;

		ImageSettings() = default;
	};

	class Image2D : public RenderResource
	{
	private:


	public:
		virtual  std::any GetHandle() = 0;
		virtual EngineGraphicsFormats GetImageFormat() = 0;
		virtual ImageLayout GetLayout() = 0;
		virtual uint32_t GetHeight() = 0;
		virtual uint32_t GetWidth() = 0;

		static Ref<Image2D> Create(const ImageSettings& settings);

	};

	class Image3D : public RenderResource
	{
	private:


	public:
		virtual  std::any GetHandle() = 0;
		virtual EngineGraphicsFormats GetImageFormat() = 0;
		virtual ImageLayout GetLayout() = 0;
		virtual uint32_t GetHeight() = 0;
		virtual uint32_t GetWidth() = 0;
		virtual uint32_t GetDepth() = 0;

	};

}

