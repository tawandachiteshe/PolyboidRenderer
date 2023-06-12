#pragma once
#include <any>

#include "Engine/Engine/Base.h"


namespace Polyboid
{
	class Image2D;
	class Texture3D;
	class Texture2D;
	class StorageBuffer;
	class UniformBuffer;

	class PipelineDescriptorSet
	{

	public:
		virtual std::any GetHandle() = 0;
		virtual void WriteUniformBuffer(uint32_t binding, const Ref<UniformBuffer>& buffer) = 0;
		virtual void WriteStorageBuffer(uint32_t binding, const Ref<StorageBuffer>& buffer) = 0;
		virtual void WriteTexture2D(uint32_t binding, const Ref<Texture2D>& texture) = 0;
		virtual void WriteTexture3D(uint32_t binding, const Ref<Texture3D>& texture) = 0;
		virtual void WriteImage2D(uint32_t binding, const Ref<Image2D>& image2d) = 0;
		virtual void Commit() = 0;
		virtual ~PipelineDescriptorSet() = default;
	};

}

