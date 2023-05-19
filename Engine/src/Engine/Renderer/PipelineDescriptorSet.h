#pragma once
#include <any>

#include "Engine/Engine/Base.h"


namespace Polyboid
{
	class Texture;
	class StorageBuffer;
	class UniformBuffer;

	class PipelineDescriptorSet
	{
	public:
		virtual std::any GetHandle() = 0;
		virtual void WriteUniformBuffer(uint32_t binding, const Ref<UniformBuffer>& buffer) = 0;
		virtual void WriteStorageBuffer(uint32_t binding, const Ref<StorageBuffer>& buffer) = 0;
		virtual void WriteTexture2D(uint32_t binding, const Ref<Texture>& texture) = 0;
		virtual void Commit() = 0;
		virtual ~PipelineDescriptorSet() = default;
	};

}

