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
		void BeginRenderPass(const Ref<RenderPass>& renderPass) override;
		void EndRenderPass() override;
		void CopyIndexBuffer(const Ref<StagingBuffer>& srcIndexBuffer, const Ref<IndexBuffer>& dstIndexBuffer) override;
		void CopyVertexBuffer(const Ref<StagingBuffer>& srcVtxBuffer, const Ref<VertexBuffer>& dstVtxBuffer) override;
		void CopyBufferToImage2D(const Ref<StagingBuffer>& stagingBuffer, const Ref<Image2D>& dstImage) override;
		void VulkanCopyBufferToCubemap(const Ref<StagingBuffer>& stagingBuffer, const Ref<Image2D>& dstImage, const std::vector<vk::BufferImageCopy>& regions);
		void TransitionImageLayout(const Ref<Image2D>& src, ImageLayout newLayout, uint32_t layerCount = 1, uint32_t mipLevel = 1) override;
		void VulkanTransitionImageLayout(vk::Image src, vk::ImageLayout oldLayout, vk::ImageLayout newLayout, uint32_t layerCount = 1, uint32_t mipLevel = 1);
		void VulkanCopyImage(const Ref<Image2D>& srcImage, vk::ImageLayout srcLayout, const Ref<Image2D>& dstImage, vk::ImageLayout dstLayout);
		void VulkanBlitImage(vk::Image srcImage, vk::ImageLayout srcLayout, vk::Image dtsImage, vk::ImageLayout dstLayout, uint32_t width, uint32_t height,uint32_t mipIndex);
		void VulkanImageBarrier(vk::Image image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout, vk::AccessFlags srcAccess, vk::AccessFlags dstAccess, vk::PipelineStageFlags srcFlags, vk::PipelineStageFlags dstFlags, uint32_t mipIndex, uint32_t mipLevels = 1);
		void VulkanGenerateMips(vk::Image image, uint32_t width, uint32_t height, uint32_t mipCount);

		void CopyHostMemoryBarrier(const Ref<StagingBuffer>& srcBuffer, const PipelineStage& stage) override;
		void SetLineWidth(float lineWidth) override;

		void CopyIndexBuffer(const Ref<StagingBuffer>& srcIndexBuffer, const IndexBuffer* dstIndexBuffer) override;
		void CopyVertexBuffer(const Ref<StagingBuffer>& srcVtxBuffer, const VertexBuffer* dstVtxBuffer) override;
		void BindIndexBuffer(const Ref<IndexBuffer>& idxBuffer) override;
		void BindVertexBuffer(const Ref<VertexBuffer>& vtxBuffer) override;
		void BindGraphicsPipeline(const Ref<GraphicsPipeline>& pipeline) override;
		void CopyUniformBuffer(const Ref<StagingBuffer>& srcUbo, const Ref<UniformBuffer>& dstUbo, const PipelineStage& stage = PipelineStage::VertexShader) override;
		void CopyStorageBuffer(const Ref<StagingBuffer>& srcUbo, const Ref<StorageBuffer>& storageBuffer, const PipelineStage& stage = PipelineStage::VertexShader) override;


		void SetViewPort(const Viewport& viewport) override;
		void SetScissor(const Rect& rect) override;
		
		void BindGraphicsDescriptorSet(uint32_t setBinding, const Ref<PipelineDescriptorSet>& set) override;
		void DrawIndexed(uint32_t count, const IndexDataType& type) override;
		void DrawArrays(uint32_t count) override;
		void GraphicsPushConstant(const Ref<GraphicsPipeline>& pipeline, ShaderType type, const void* data, uint32_t size, uint32_t offset) override;
		void KomputePushConstant(const Ref<KomputePipeline>& pipeline, const void* data, uint32_t size,
			uint32_t offset) override;

		void Reset() override;

		void DispatchKompute(const glm::uvec3& workGroup) override;
		void BindKomputeDescriptorSet(uint32_t setBinding, const Ref<PipelineDescriptorSet>& set) override;
		void BindKomputePipline(const Ref<KomputePipeline>& pipeline) override;

		std::any GetHandle() override;

		~VulkanCommandBuffer() override;
	};

}

	

