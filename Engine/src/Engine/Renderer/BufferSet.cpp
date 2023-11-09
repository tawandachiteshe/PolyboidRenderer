#include "boidpch.h"
#include "BufferSet.h"

#include "RenderCommand.h"
#include "Platform/Vulkan/Buffers.h"
#include "Platform/Vulkan/VulkanFramebuffer.h"

namespace Polyboid
{
	UniformBufferSet::UniformBufferSet(uint32_t size)
	{
		for (uint32_t i = 0; i < RenderCommand::GetMaxFramesInFlight(); ++i)
		{
			m_Buffers.push_back(UniformBuffer::Create(size));
		}

	}

	void UniformBufferSet::Recreate()
	{
		for (const auto& buffer : m_Buffers)
		{
			buffer.As<VulkanUniformBuffer>()->Recreate();
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

	void UniformBufferSet::Destroy()
	{
	}

	RenderResourceType UniformBufferSet::GetRenderResourceType()
	{
		return RenderResourceType::UniformBuffer;
	}


	// Storage buffers here
	StorageBufferSet::StorageBufferSet(uint32_t size)
	{
		for (uint32_t i = 0; i < RenderCommand::GetMaxFramesInFlight(); ++i)
		{
			m_Buffers.push_back(StorageBuffer::Create(size));
		}
	}

	void StorageBufferSet::Recreate()
	{
		for (const auto& buffer : m_Buffers)
		{
			buffer.As<VulkanShaderStorage>()->Recreate();
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

	void StorageBufferSet::Destroy()
	{
	}

	RenderResourceType StorageBufferSet::GetRenderResourceType()
	{
		return RenderResourceType::StorageBuffer;
	}

	VertexBufferSet::VertexBufferSet(uint32_t size)
	{
		for (uint32_t i = 0; i < RenderCommand::GetMaxFramesInFlight(); ++i)
		{
			m_Buffers.push_back(VertexBuffer::Create(size));
		}
	}

	void VertexBufferSet::Recreate()
	{
		for (const auto& buffer : m_Buffers)
		{
			buffer.As<VulkanShaderStorage>()->Recreate();
		}
	}

	void VertexBufferSet::SetLayout(const BufferLayout& layout)
	{
		for (const auto& buffer : m_Buffers)
		{
			buffer->SetLayout(layout);
		}
	}

	Ref<VertexBufferSet> VertexBufferSet::Create(uint32_t size)
	{
		return CreateRef<VertexBufferSet>(size);
	}

	Ref<VertexBuffer> VertexBufferSet::Get(uint32_t frame)
	{
		return m_Buffers.at(frame);
	}


	//Staging buffer
	StagingBufferSet::StagingBufferSet(uint32_t size)
	{
		for (uint32_t i = 0; i < RenderCommand::GetMaxFramesInFlight(); ++i)
		{
			m_Buffers.push_back(StagingBuffer::Create(size));
		}
	}

	void StagingBufferSet::Recreate()
	{
		for (const auto& buffer : m_Buffers)
		{
			buffer.As<VulkanShaderStorage>()->Recreate();
		}
	}

	void FrameBufferSet::Recreate()
	{
		for (const auto& buffer : m_Buffers)
		{
			buffer.As<VulkanFramebuffer>()->Recreate();
		}
	}

	void FrameBufferSet::ReSize(uint32_t width, uint32_t height)
	{
		for (const auto& buffer : m_Buffers)
		{
			buffer.As<VulkanFramebuffer>()->ReSize(width, height);
		}
	}


	Ref<StagingBufferSet> StagingBufferSet::Create(uint32_t size)
	{
		return CreateRef<StagingBufferSet>(size);
	}

	void StagingBufferSet::SetData(const void* data)
	{
		m_Buffers.at(RenderCommand::GetCurrentFrame())->SetData(data);
	}

	void StagingBufferSet::SetData(const void* data, uint32_t size)
	{
		m_Buffers.at(RenderCommand::GetCurrentFrame())->SetData(data, size);
	}

	Ref<StagingBuffer> StagingBufferSet::Get(uint32_t frame)
	{
		return m_Buffers.at(frame);
	}


	//Frame buffers
	FrameBufferSet::FrameBufferSet(const Ref<RenderPass>& renderPass)
	{
		for (uint32_t i = 0; i < RenderCommand::GetMaxFramesInFlight(); ++i)
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

	FrameBufferSet::~FrameBufferSet()
	{

	}
}
