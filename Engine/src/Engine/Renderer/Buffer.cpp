#include "boidpch.h"
#include "Buffer.h"

#include "RenderAPI.h"


namespace Polyboid
{
	Ref<StorageBuffer> StorageBuffer::Create(uint32_t size)
	{
		return RenderAPI::Get()->CreateStorageBuffer(size);
	}

	Ref<VertexBuffer> VertexBuffer::Create(const void* data, uint32_t size)
	{
		return RenderAPI::Get()->CreateVertexBuffer(data, size);
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		return RenderAPI::Get()->CreateIndexBuffer(IndexDataType::UnsignedInt, count, indices);
	}

	Ref<StagingBuffer> StagingBuffer::Create(uint32_t size)
	{
		return RenderAPI::Get()->CreateStagingBuffer(size);
	}
}
