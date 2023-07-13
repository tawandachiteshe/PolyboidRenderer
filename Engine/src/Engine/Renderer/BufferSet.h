#pragma once
#include "Buffer.h"
#include "UniformBuffer.h"
#include "Engine/Engine/Base.h"


namespace Polyboid
{
	class RenderPass;
	class Framebuffer;

	class UniformBufferSet : public RenderResource
	{
		std::vector<Ref<UniformBuffer>> m_Buffers;
	public:
		UniformBufferSet(uint32_t size);
		void Recreate() override;
		static Ref<UniformBufferSet> Create(uint32_t size);
		Ref<UniformBuffer> Get(uint32_t frame);
		void Destroy() override;
		RenderResourceType GetRenderResourceType() override;
	};

	class StorageBufferSet : public RenderResource
	{
		std::vector<Ref<StorageBuffer>> m_Buffers;
	public:
		StorageBufferSet(uint32_t size);
		void Recreate() override;
		static Ref<StorageBufferSet> Create(uint32_t size);
		Ref<StorageBuffer> Get(uint32_t frame);
		void Destroy() override;
		RenderResourceType GetRenderResourceType() override;
	};

	class VertexBufferSet
	{
		std::vector<Ref<VertexBuffer>> m_Buffers;
	public:
		VertexBufferSet(uint32_t size);
		void Recreate();
		void SetLayout(const BufferLayout& layout);
		static Ref<VertexBufferSet> Create(uint32_t size);
		Ref<VertexBuffer> Get(uint32_t frame);
	};


	class StagingBufferSet
	{
		std::vector<Ref<StagingBuffer>> m_Buffers;
	public:
		StagingBufferSet(uint32_t size);
		void Recreate();
		static Ref<StagingBufferSet> Create(uint32_t size);
		void SetData(const void* data);
		void SetData(const void* data, uint32_t size);

		Ref<StagingBuffer> Get(uint32_t frame);
	};


	class FrameBufferSet
	{
		std::vector<Ref<Framebuffer>> m_Buffers;
	public:
		FrameBufferSet(const Ref<RenderPass>& renderPass);
		void Recreate();
		void ReSize(uint32_t width, uint32_t height);
		static Ref<FrameBufferSet> Create(const Ref<RenderPass>& renderPass);
		Ref<Framebuffer> Get(uint32_t frame);
	};

}

