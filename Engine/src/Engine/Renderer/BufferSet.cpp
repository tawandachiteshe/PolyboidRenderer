#include "boidpch.h"
#include "BufferSet.h"

#include "Renderer.h"

namespace Polyboid
{
	UniformBufferSet::UniformBufferSet(uint32_t size)
	{
		for (uint32_t i = 0; i < Renderer::GetMaxFramesInFlight(); ++i)
		{
			m_Buffers.push_back(UniformBuffer::Create(size));
		}

	}

	Ref<UniformBufferSet> UniformBufferSet::Create(uint32_t size)
	{
		return CreateRef<UniformBufferSet>(size);
	}

	Ref<UniformBuffer> UniformBufferSet::Get(uint32_t frame)
	{
		return m_Buffers.at(frame);
	}


	// Storage buffers here
	StorageBufferSet::StorageBufferSet(uint32_t size)
	{
		for (uint32_t i = 0; i < Renderer::GetMaxFramesInFlight(); ++i)
		{
			m_Buffers.push_back(StorageBuffer::Create(size));
		}
	}

	Ref<StorageBufferSet> StorageBufferSet::Create(uint32_t size)
	{
		return CreateRef<StorageBufferSet>(size);
	}

	Ref<StorageBuffer> StorageBufferSet::Get(uint32_t frame)
	{
		return m_Buffers.at(frame);
	}

 
	//Staging buffer
	StagingBufferSet::StagingBufferSet(uint32_t size)
	{
		for (uint32_t i = 0; i < Renderer::GetMaxFramesInFlight(); ++i)
		{
			m_Buffers.push_back(StagingBuffer::Create(size));
		}
	}

	Ref<StagingBufferSet> StagingBufferSet::Create(uint32_t size)
	{
		return CreateRef<StagingBufferSet>(size);
	}

	Ref<StagingBuffer> StagingBufferSet::Get(uint32_t frame)
	{
		return m_Buffers.at(frame);
	}


	//Frame buffers
	FrameBufferSet::FrameBufferSet(const Ref<RenderPass>& renderPass)
	{
		for (uint32_t i = 0; i < Renderer::GetMaxFramesInFlight(); ++i)
		{
			m_Buffers.push_back(Framebuffer::Create(renderPass));
		}
	}

	Ref<FrameBufferSet> FrameBufferSet::Create(const Ref<RenderPass>& renderPass)
	{
		return CreateRef<FrameBufferSet>(renderPass);
	}

	Ref<Framebuffer> FrameBufferSet::Get(uint32_t frame)
	{
		return m_Buffers.at(frame);
	}
}
