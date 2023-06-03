#pragma once


#include <vulkan/vulkan.hpp>
#include "Engine/Renderer/CommandBufferSet.h"
#include <any>

namespace Polyboid
{
	class VulkanRenderPass;
	class VkSwapChain;
	class VulkanSemaphore;
	class VulkanFence;
	class VulkanCommandBufferSet;
	class VkRenderAPI;

	class VulkanCommandBuffer : public CommandBuffer
	{
	private:
		vk::CommandBuffer m_CommandBuffer;
		const VulkanCommandBufferSet* m_CommandList = nullptr;
		const VkRenderAPI* m_Context;
		Ref<VulkanRenderPass> m_RenderPass = nullptr;

	public:

		void Destroy(const vk::Device& device);
		VulkanCommandBuffer(const VkRenderAPI* context, const VulkanCommandBufferSet* commands);
		void Init(const VkRenderAPI* context, const VulkanCommandBufferSet* commands);
		virtual void Recreate();
		void Begin() override;
		void End() override;
		void BeginRenderPass(const Ref<RenderPass>& renderPass, const Ref<Framebuffer>& framebuffer) override;
		void EndRenderPass() override;
		void CopyIndexBuffer(const Ref<StagingBuffer>& srcIndexBuffer, const Ref<IndexBuffer>& dstIndexBuffer) override;
		void CopyVertexBuffer(const Ref<StagingBuffer>& srcVtxBuffer, const Ref<VertexBuffer>& dstVtxBuffer) override;
		void CopyBufferToImage2D(const Ref<StagingBuffer>& stagingBuffer, const Ref<Image2D>& dstImage) override;
		void TransitionImageLayout(const Ref<Image2D>& src, ImageLayout newLayout) override;
		

		void CopyIndexBuffer(const Ref<StagingBuffer>& srcIndexBuffer, const IndexBuffer* dstIndexBuffer) override;
		void CopyVertexBuffer(const Ref<StagingBuffer>& srcVtxBuffer, const VertexBuffer* dstVtxBuffer) override;
		void BindIndexBuffer(const Ref<IndexBuffer>& idxBuffer) override;
		void BindVertexBuffer(const Ref<VertexBuffer>& vtxBuffer) override;
		void BindGraphicsPipeline(const Ref<PipelineState>& pipeline) override;
		void CopyUniformBuffer(const Ref<StagingBuffer>& srcUbo, const Ref<UniformBuffer>& dstUbo) override;
		void CopyStorageBuffer(const Ref<StagingBuffer>& srcUbo, const Ref<StorageBuffer>& storageBuffer) override;


		void SetViewPort(const Viewport& viewport) override;
		void SetScissor(const Rect& rect) override;
		
		void BindDescriptorSet(uint32_t setBinding, const Ref<PipelineDescriptorSet>& set) override;
		void DrawIndexed(uint32_t count, const IndexDataType& type) override;
		void DrawArrays(uint32_t count) override;
		void PushConstant(const Ref<PipelineState>& pipeline, ShaderType type, const void* data, uint32_t size, uint32_t offset) override;

		void Reset() override;

		std::any GetHandle() override;

		~VulkanCommandBuffer() override;
	};

}

	

