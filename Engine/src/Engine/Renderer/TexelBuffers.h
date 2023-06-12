﻿#pragma once
#include "Texture2D.h"


namespace Polyboid
{


	struct TexelBufferSettings
	{
		uint32_t height = 1;
		uint32_t width = 1;
		EngineGraphicsFormats format;

		TexelBufferSettings() = default;
	};



	class TexelStorageBuffer
	{
	public:

		virtual  std::any GetHandle() = 0;
		virtual EngineGraphicsFormats GetImageFormat() = 0;
		virtual ImageLayout GetLayout() = 0;
		virtual uint32_t GetHeight() = 0;
		virtual uint32_t GetWidth() = 0;

		virtual ~TexelStorageBuffer() {}
	};

	class TexelUniformBuffer
	{
	public:

		virtual  std::any GetHandle() = 0;
		virtual EngineGraphicsFormats GetImageFormat() = 0;
		virtual ImageLayout GetLayout() = 0;
		virtual uint32_t GetHeight() = 0;
		virtual uint32_t GetWidth() = 0;

		virtual ~TexelUniformBuffer() {}

	};
}

