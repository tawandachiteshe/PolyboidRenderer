#include "boidpch.h"
#include "TexelBuffers.h"

#include "RenderAPI.h"


namespace Polyboid
{
	Ref<TexelStorageBuffer> TexelStorageBuffer::Create(const TexelBufferSettings& settings)
	{
		return RenderAPI::Get()->CreateTexelStorageBuffer(settings);
	}

	Ref<TexelUniformBuffer> TexelUniformBuffer::Create(const TexelBufferSettings& settings)
	{
		return RenderAPI::Get()->CreateTexelUniformBuffer(settings);
	}
}
