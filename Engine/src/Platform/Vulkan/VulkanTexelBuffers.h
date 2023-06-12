#pragma once
#include "Engine/Renderer/TexelBuffers.h"


namespace Polyboid
{
	class VulkanTexelStorageBuffer : public TexelStorageBuffer
	{
	public:

		VulkanTexelStorageBuffer(const TexelBufferSettings& settings);

		std::any GetHandle() override;
		EngineGraphicsFormats GetImageFormat() override;
		ImageLayout GetLayout() override;
		uint32_t GetHeight() override;
		uint32_t GetWidth() override;
		~VulkanTexelStorageBuffer() override;
	};

	class VulkanTexelUniformBuffer : public TexelUniformBuffer
	{
	public:
		VulkanTexelUniformBuffer(const TexelBufferSettings& settings);

		std::any GetHandle() override;
		EngineGraphicsFormats GetImageFormat() override;
		ImageLayout GetLayout() override;
		uint32_t GetHeight() override;
		uint32_t GetWidth() override;
	};

}

