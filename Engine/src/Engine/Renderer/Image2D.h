﻿#pragma once
#include <any>
#include <vulkan/vulkan.hpp>


namespace Polyboid
{
	enum class EngineGraphicsFormats;

	enum class ImageLayout
	{
		ColorAttachmentOptimal,
		TransferDstOptimal,
		DepthAttachmentOptimal,
		ShaderReadOptimal,
		Undefined
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
		DepthStencilAttachmentSampling
	};

	struct ImageSettings
	{
		ImageUsage usage = ImageUsage::Sampling;
		uint32_t height = 1;
		uint32_t width = 1;
		uint32_t sampleCount = 1;
		uint32_t mipCount = 1;
		bool generateMips = false;
		EngineGraphicsFormats format;
		ImageLayout layout = ImageLayout::Undefined;
	};

	class Image2D
	{
	private:


	public:
		virtual  std::any GetHandle() = 0;
		virtual ~Image2D() = default;
		virtual EngineGraphicsFormats GetImageFormat() = 0;
		virtual ImageLayout GetLayout() = 0;
		virtual uint32_t GetHeight() = 0;
		virtual uint32_t GetWidth() = 0;

	};

}
