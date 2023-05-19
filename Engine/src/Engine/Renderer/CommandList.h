#pragma once
#include <any>
#include <cstdint>

#include "Rect.h"
#include "Viewport.h"
#include "Engine/Engine/Base.h"

namespace Polyboid
{
	enum class IndexDataType;
	class PipelineDescriptorSet;
	class PipelineState;
	enum class ImageLayout;
	class Image2D;
	class StagingBuffer;
	class VertexBuffer;
	class IndexBuffer;
	class Framebuffer;
	class Swapchain;
	class RenderPass;

	class CommandBuffer
	{
	public:


		virtual void Begin() = 0;
		virtual void End() = 0;
		virtual void BeginRenderPass(const Ref<RenderPass>& renderPass, const Ref<Framebuffer>& framebuffer) = 0;
		virtual void EndRenderPass() = 0;
		virtual void Reset() = 0;
		virtual std::any GetHandle() = 0;
		virtual void CopyIndexBuffer(const Ref<StagingBuffer>& srcIndexBuffer, const Ref<IndexBuffer>& dstIndexBuffer) = 0;
		virtual void CopyVertexBuffer(const Ref<StagingBuffer>& srcVtxBuffer, const Ref<VertexBuffer>& dstVtxBuffer) = 0;
		virtual void CopyIndexBuffer(const Ref<StagingBuffer>& srcIndexBuffer, const IndexBuffer* dstIndexBuffer) = 0;
		virtual void CopyVertexBuffer(const Ref<StagingBuffer>& srcVtxBuffer, const VertexBuffer* dstVtxBuffer) = 0;
		virtual void CopyBufferToImage2D(const Ref<StagingBuffer>& stagingBuffer, const Ref<Image2D>& dstImage) = 0;
		virtual void TransitionImageLayout(const Ref<Image2D>& src, ImageLayout newLayout) = 0;

		virtual void SetViewPort(const Viewport& viewport) = 0;
		virtual void SetScissor(const Rect& rect) = 0;

		virtual void BindIndexBuffer(const Ref<IndexBuffer>& idxBuffer) = 0;
		virtual void BindVertexBuffer(const Ref<VertexBuffer>& vtxBuffer) = 0;
		virtual void BindGraphicsPipeline(const Ref<PipelineState>& pipeline) = 0;
		virtual void BindDescriptorSet(uint32_t setBinding, const Ref<PipelineDescriptorSet>& set) = 0;
		virtual void DrawIndexed(uint32_t count, const IndexDataType& type) = 0;
		virtual void DrawArrays(uint32_t count) = 0;

		virtual ~CommandBuffer() = default;

		static Ref<CommandBuffer> Create();

	};

	struct CommandListSettings
	{
		uint32_t ImageCount = 3;
	};

	class CommandList
	{
	public:

		virtual Ref<CommandBuffer> GetCommandBufferAt(uint32_t index = 0) = 0;
		virtual ~CommandList() = default;

		static Ref<CommandList> Create(const CommandListSettings& settings);

	};

}
