#pragma once
#include <any>
#include <cstdint>
#include <glm/vec3.hpp>

#include "Rect.h"
#include "Viewport.h"
#include "RenderResource.h"
#include "Engine/Engine/Base.h"

namespace Polyboid
{
	class KomputePipeline;
	class StorageBuffer;
	enum class ShaderType : uint8_t;
	class UniformBuffer;
	enum class IndexDataType;
	class PipelineDescriptorSet;
	class GraphicsPipeline;
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
		virtual void CopyUniformBuffer(const Ref<StagingBuffer>& srcUbo, const Ref<UniformBuffer>& dstUbo) = 0;
		virtual void CopyStorageBuffer(const Ref<StagingBuffer>& srcUbo, const Ref<StorageBuffer>& storageBuffer) = 0;

		virtual void CopyHostMemoryBarrier(const Ref<StagingBuffer>& srcBuffer) = 0;

		virtual void SetViewPort(const Viewport& viewport) = 0;
		virtual void SetScissor(const Rect& rect) = 0;
		virtual void DispatchKompute(const glm::uvec3& workGroup) = 0;
		virtual void BindKomputePipline(const Ref<KomputePipeline>& pipeline) = 0;
		virtual void BindKomputeDescriptorSet(uint32_t setBinding, const Ref<PipelineDescriptorSet>& set) = 0;

		virtual void BindIndexBuffer(const Ref<IndexBuffer>& idxBuffer) = 0;
		virtual void BindVertexBuffer(const Ref<VertexBuffer>& vtxBuffer) = 0;
		virtual void BindGraphicsPipeline(const Ref<GraphicsPipeline>& pipeline) = 0;
		virtual void BindGraphicsDescriptorSet(uint32_t setBinding, const Ref<PipelineDescriptorSet>& set) = 0;
		virtual void DrawIndexed(uint32_t count, const IndexDataType& type) = 0;
		virtual void DrawArrays(uint32_t count) = 0;
		virtual void GraphicsPushConstant(const Ref<GraphicsPipeline>& pipeline, ShaderType type, const void* data, uint32_t size, uint32_t offset) = 0;
		virtual void KomputePushConstant(const Ref<KomputePipeline>& pipeline, const void* data, uint32_t size, uint32_t offset) = 0;
		virtual void SetLineWidth(float lineWidth = 1.0) = 0;

		virtual ~CommandBuffer() = default;

		static Ref<CommandBuffer> Create();

	};

	enum class CommandType
	{
		OneTime,
		ManyTime
	};

	struct CommandListSettings
	{
		uint32_t ImageCount = 3;
		CommandType SubmissionType = CommandType::OneTime;
	};

	class CommandBufferSet : public RenderResource
	{

	public:

		virtual Ref<CommandBuffer> GetCommandBufferAt(uint32_t index = 0) = 0;
		virtual ~CommandBufferSet() = default;
		virtual CommandListSettings& GetSettings() = 0;

		static Ref<CommandBufferSet> Create(const CommandListSettings& settings);

	};

}
