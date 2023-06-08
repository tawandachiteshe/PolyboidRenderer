#pragma once


#include "Buffer.h"
#include "Engine/Engine/Base.h"
#include "RenderAPI.h"

namespace Polyboid
{
	class VertexBufferSet;

	class VertexBufferArray : public RenderResource
    {
	public:
		virtual bool IsUsingVertexPulling() = 0;
		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vbuffer) = 0;
		virtual void AddVertexBufferSet(const Ref<VertexBufferSet>& vbufferSet) = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& iBuffer) = 0;
		virtual void SetIndexBuffer(const Ref<StorageBuffer>& buffer, uint32_t count, const IndexDataType& type = IndexDataType::UnsignedInt) = 0;
		virtual Ref<IndexBuffer> GetIndexBuffer() = 0;
		virtual IndexDataType GetIndexDataType() = 0;
		virtual uint32_t GetIndexCount() = 0;
		virtual void SetIndexCount(uint32_t count) = 0;

		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		static Ref<VertexBufferArray> Create();
    };

    
}
