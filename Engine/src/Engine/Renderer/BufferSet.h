#pragma once
#include "Buffer.h"
#include "UniformBuffer.h"
#include "Engine/Engine/Base.h"


namespace Polyboid
{
	class UniformBufferSet
	{
		std::vector<Ref<UniformBuffer>> m_Buffers;
	public:
		UniformBufferSet(uint32_t size);
		static Ref<UniformBufferSet> Create(uint32_t size);
		Ref<UniformBuffer> Get(uint32_t frame);
	};

	class StorageBufferSet
	{
		std::vector<Ref<StorageBuffer>> m_Buffers;
	public:
		StorageBufferSet(uint32_t size);
		static Ref<StorageBufferSet> Create(uint32_t size);
		Ref<StorageBuffer> Get(uint32_t frame);
	};


	class StagingBufferSet
	{
		std::vector<Ref<StagingBuffer>> m_Buffers;
	public:
		StagingBufferSet(uint32_t size);
		static Ref<StagingBufferSet> Create(uint32_t size);
		Ref<StagingBuffer> Get(uint32_t frame);
	};

}

