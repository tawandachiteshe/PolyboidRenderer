#include "boidpch.h"
#include "UniformBuffer.h"

#include "RenderAPI.h"
#include "Engine/Engine/Base.h"


namespace Polyboid
{
	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size)
	{
		return RenderAPI::Get()->CreateUniformBuffer(size, 0);
	}
}
