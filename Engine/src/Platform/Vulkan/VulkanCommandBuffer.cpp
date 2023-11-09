#include "boidpch.h"
#include "VulkanCommandBuffer.h"

#include <glm/common.hpp>


#include "VkRenderAPI.h"
#include "VulkanCommandBufferSet.h"
#include "VulkanFramebuffer.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanKomputePipeline.h"
#include "VulkanPipelineDescriptorSet.h"
#include "VulkanRenderPass.h"
#include "Engine/Engine/ImguiSetup.h"
#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/Image2D.h"
#include "Engine/Renderer/PipelineDescriptorSet.h"
#include "Engine/Renderer/GraphicsPipeline.h"
#include "Engine/Renderer/UniformBuffer.h"
#include "vulkan/vulkan.hpp"
#include "Utils/VulkanDevice.h"
#include "Utils/Common.h"

namespace Polyboid
{
	void VulkanCommandBuffer::Destroy(const vk::Device& device)
	{
		device.freeCommandBuffers(m_CommandList->m_CommandList,{m_CommandBuffer});
	}

	VulkanCommandBuffer::VulkanCommandBuffer(const VkRenderAPI* context, const VulkanCommandBufferSet* commands): m_CommandList(commands), m_Context(context)
	{


		Init(context, commands);

	}

	void VulkanCommandBuffer::Init(const VkRenderAPI* context, const VulkanCommandBufferSet* commands)
	{
		const vk::Device device = *context->GetDevice();

		vk::CommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = vk::StructureType::eCommandBufferAllocateInfo;
		allocInfo.commandPool = commands->m_CommandList;
		allocInfo.level = vk::CommandBufferLevel::ePrimary;
		allocInfo.commandBufferCount = 1;

		auto [result, commandbuffer] = device.allocateCommandBuffers(allocInfo);
		vk::resultCheck(result, "Failed to  alloc command buffer");
		m_CommandBuffer = commandbuffer.at(0);
	}


	void VulkanCommandBuffer::Recreate()
	{
		Init(m_Context, m_CommandList);
	}

	void VulkanCommandBuffer::Begin()
	{
		vk::Result result;
		result = m_CommandBuffer.reset();
		vk::resultCheck(result, "Failed to reset command buffer");

		vk::CommandBufferBeginInfo beginInfo{};
		beginInfo.sType = vk::StructureType::eCommandBufferBeginInfo;
		beginInfo.flags = m_CommandList->m_Settings.SubmissionType == CommandType::ManyTime ? vk::CommandBufferUsageFlagBits::eSimultaneousUse : vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
		beginInfo.pInheritanceInfo = nullptr;
		
		 result = m_CommandBuffer.begin(beginInfo);
		vk::resultCheck(result, "Failed to begin command buffer");

	}

	void VulkanCommandBuffer::End()
	{
		 auto result = m_CommandBuffer.end();
		 vk::resultCheck(result, "Failed to end command buffer");
	}

	void VulkanCommandBuffer::BeginRenderPass(const Ref<RenderPass>& renderPass)
	{

		auto vkRenderpass = renderPass.As<VulkanRenderPass>();
		const vk::RenderPassBeginInfo renderPassInfo = vkRenderpass->GetRenderBeginInfo();
		m_RenderPass = vkRenderpass;
		m_CommandBuffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
		
	}

	void VulkanCommandBuffer::EndRenderPass()
	{
		
		m_CommandBuffer.endRenderPass();
	}

	void VulkanCommandBuffer::CopyIndexBuffer(const Ref<StagingBuffer>& srcIndexBuffer,
		const Ref<IndexBuffer>& dstIndexBuffer)
	{
		vk::BufferCopy copyRegion{};
		copyRegion.dstOffset = 0;
		copyRegion.srcOffset = 0;

		if (srcIndexBuffer->GetSizeInBytes() != dstIndexBuffer->GetSizeInBytes())
		{
			__debugbreak();
		}

		copyRegion.size = dstIndexBuffer->GetSizeInBytes();
		const auto dstBuffer = std::any_cast<vk::Buffer>(dstIndexBuffer->GetHandle());
		const auto srcBuffer = std::any_cast<vk::Buffer>(srcIndexBuffer->GetHandle());

		m_CommandBuffer.copyBuffer(srcBuffer, dstBuffer, 1, &copyRegion);
	}

	void VulkanCommandBuffer::CopyVertexBuffer(const Ref<StagingBuffer>& srcVtxBuffer,
		const Ref<VertexBuffer>& dstVtxBuffer)
	{
		CopyHostMemoryBarrier(srcVtxBuffer, PipelineStage::Transfer);
		vk::BufferCopy copyRegion{};
		copyRegion.dstOffset = 0;
		copyRegion.srcOffset = 0;

		if (srcVtxBuffer->GetSizeInBytes() != dstVtxBuffer->GetSizeInBytes())
		{
			__debugbreak();
		}

		copyRegion.size = dstVtxBuffer->GetSizeInBytes();
		const auto dstBuffer = std::any_cast<vk::Buffer>(dstVtxBuffer->GetHandle());
		const auto srcBuffer = std::any_cast<vk::Buffer>(srcVtxBuffer->GetHandle());

		m_CommandBuffer.copyBuffer(srcBuffer, dstBuffer, 1, &copyRegion);



	}

	void VulkanCommandBuffer::CopyBufferToImage2D(const Ref<StagingBuffer>& stagingBuffer, const Ref<Image2D>& dstImage)
	{
		vk::BufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;

		region.imageSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;

		region.imageOffset = vk::Offset3D{ 0, 0, 0 };
		region.imageExtent = vk::Extent3D{
			dstImage->GetWidth(),
			dstImage->GetHeight(),
			1
		};

		m_CommandBuffer.copyBufferToImage(
			std::any_cast<vk::Buffer>(stagingBuffer->GetHandle()),
			std::any_cast<vk::Image>(dstImage->GetHandle()),
			vk::ImageLayout::eTransferDstOptimal, 1, &region);


	}

	void VulkanCommandBuffer::VulkanCopyBufferToCubemap(const Ref<StagingBuffer>& stagingBuffer,
		const Ref<Image2D>& dstImage, const std::vector<vk::BufferImageCopy>& regions)
	{
		m_CommandBuffer.copyBufferToImage(
			std::any_cast<vk::Buffer>(stagingBuffer->GetHandle()),
			std::any_cast<vk::Image>(dstImage->GetHandle()),
			vk::ImageLayout::eTransferDstOptimal, regions);
	}



	void VulkanCommandBuffer::TransitionImageLayout(const Ref<Image2D>& src, ImageLayout newLayout, uint32_t layerCount, uint32_t mipLevel )
	{

		vk::ImageMemoryBarrier barrier{};
		barrier.oldLayout = Utils::ConvertToVulkanLayout(src->GetLayout());
		barrier.newLayout = Utils::ConvertToVulkanLayout(newLayout);

		barrier.srcQueueFamilyIndex = 0;
		barrier.dstQueueFamilyIndex = 0;
		barrier.image = std::any_cast<vk::Image>(src->GetHandle());
		barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = mipLevel;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = layerCount;

		barrier.srcAccessMask = vk::AccessFlagBits::eNone; //TODO: important wangu
		barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite; //TODO: important wangu

	 	const vk::PipelineStageFlags dstStageFlags = vk::PipelineStageFlagBits::eTransfer; //TODO: importang again we need to know;
		const vk::PipelineStageFlags srcStageFlags = vk::PipelineStageFlagBits::eTopOfPipe; //TODO: importang again we need to know;

		constexpr auto dependencyFlags = static_cast<vk::DependencyFlags>(0);

		m_CommandBuffer.pipelineBarrier(
			srcStageFlags,
			dstStageFlags,
			dependencyFlags, 
			0, 
			nullptr,
			0,
			nullptr,
			1, &barrier);

		
	}

	void VulkanCommandBuffer::VulkanTransitionImageLayout(vk::Image src, vk::ImageLayout oldLayout, vk::ImageLayout newLayout,
		uint32_t layerCount, uint32_t mipLevel)
	{
		vk::ImageMemoryBarrier barrier{};
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;

		barrier.srcQueueFamilyIndex = 0;
		barrier.dstQueueFamilyIndex = 0;
		barrier.image = src;
		barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = mipLevel;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = layerCount;

		barrier.srcAccessMask = vk::AccessFlagBits::eNone; //TODO: important wangu
		barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite; //TODO: important wangu

		const vk::PipelineStageFlags dstStageFlags = vk::PipelineStageFlagBits::eTransfer; //TODO: importang again we need to know;
		const vk::PipelineStageFlags srcStageFlags = vk::PipelineStageFlagBits::eTopOfPipe; //TODO: importang again we need to know;

		constexpr auto dependencyFlags = static_cast<vk::DependencyFlags>(0);

		m_CommandBuffer.pipelineBarrier(
			srcStageFlags,
			dstStageFlags,
			dependencyFlags,
			0,
			nullptr,
			0,
			nullptr,
			1, &barrier);
	}

	void VulkanCommandBuffer::VulkanCopyImage(const Ref<Image2D>& srcImage, vk::ImageLayout srcLayout,
		const Ref<Image2D>& dstImage, vk::ImageLayout dstLayout)
	{

		vk::ImageCopy copyRegions{};
		copyRegions.extent.width = srcImage->GetWidth();
		copyRegions.extent.height = srcImage->GetHeight();
		copyRegions.extent.depth = 1;

		copyRegions.srcOffset = vk::Offset3D{};
		copyRegions.srcSubresource.layerCount = 1;
		copyRegions.srcSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
		copyRegions.srcSubresource.mipLevel = 0;
		copyRegions.srcSubresource.baseArrayLayer = 0;

		copyRegions.dstOffset = vk::Offset3D{};
		copyRegions.dstSubresource.layerCount = 1;
		copyRegions.dstSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
		copyRegions.dstSubresource.mipLevel = 0;
		copyRegions.dstSubresource.baseArrayLayer = 0;

		m_CommandBuffer.copyImage(std::any_cast<vk::Image>(srcImage->GetHandle()), srcLayout, std::any_cast<vk::Image>(dstImage->GetHandle()), dstLayout, { copyRegions });

	}




	void VulkanCommandBuffer::VulkanBlitImage(vk::Image srcImage, vk::ImageLayout srcLayout, vk::Image dtsImage,
	                                          vk::ImageLayout dstLayout, uint32_t width, uint32_t height, uint32_t mipIndex)
	{
		if (mipIndex == 0)
		{
			__debugbreak();
		}

		vk::ImageBlit region{};
		region.srcSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
		region.srcSubresource.mipLevel = mipIndex - 1;
		region.srcSubresource.layerCount = 1;
		region.srcOffsets[1].x = glm::max(width >> (mipIndex - 1), 1u);
		region.srcOffsets[1].y = glm::max(height >> (mipIndex - 1), 1u);
		region.srcOffsets[1].z = 1;

		//Dst
		region.dstSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
		region.dstSubresource.mipLevel = mipIndex;
		region.dstSubresource.layerCount = 1;
		region.dstOffsets[1].x = glm::max(width >> (mipIndex ), 1u);
		region.dstOffsets[1].y = glm::max(height >> (mipIndex), 1u);
		region.dstOffsets[1].z = 1;

		m_CommandBuffer.blitImage(srcImage, srcLayout, dtsImage, dstLayout, { region }, vk::Filter::eLinear);


	}

	void VulkanCommandBuffer::VulkanImageBarrier(vk::Image image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout, vk::AccessFlags srcAccess, vk::AccessFlags dstAccess, vk::PipelineStageFlags srcFlags, vk::PipelineStageFlags dstFlags, uint32_t mipIndex, uint32_t mipLevels)
	{
		vk::ImageMemoryBarrier barrier{};
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;

		barrier.srcQueueFamilyIndex = 0;
		barrier.dstQueueFamilyIndex = 0;
		barrier.image = image;
		barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		barrier.subresourceRange.baseMipLevel = mipIndex;
		barrier.subresourceRange.levelCount = mipLevels;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		barrier.srcAccessMask = srcAccess; //TODO: important wangu
		barrier.dstAccessMask = dstAccess; //TODO: important wangu

		const vk::PipelineStageFlags srcStageFlags = srcFlags; //TODO: importang again we need to know;
		const vk::PipelineStageFlags dstStageFlags = dstFlags; //TODO: importang again we need to know;
		 

		constexpr auto dependencyFlags = static_cast<vk::DependencyFlags>(0);

		m_CommandBuffer.pipelineBarrier(
			srcStageFlags,
			dstStageFlags,
			dependencyFlags,
			{},
			{},
			{barrier});
	}

	void VulkanCommandBuffer::VulkanGenerateMips(vk::Image image, uint32_t width, uint32_t height, uint32_t mipCount)
	{
		

		for (uint32_t i = 1; i < mipCount; ++i)
		{


			VulkanImageBarrier(image, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal,
				vk::AccessFlagBits::eNone, vk::AccessFlagBits::eTransferWrite,
				vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eTransfer, i, 1);


			VulkanBlitImage(image, vk::ImageLayout::eTransferSrcOptimal, image, vk::ImageLayout::eTransferDstOptimal, width, height, i);


			VulkanImageBarrier(image, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eTransferSrcOptimal,
				vk::AccessFlagBits::eTransferWrite, vk::AccessFlagBits::eTransferRead,
				vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eTransfer, i);

		}


		VulkanImageBarrier(image, vk::ImageLayout::eTransferSrcOptimal, vk::ImageLayout::eShaderReadOnlyOptimal,
			vk::AccessFlagBits::eTransferRead, vk::AccessFlagBits::eShaderRead,
			vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader, 0, mipCount);
	}

	void VulkanCommandBuffer::CopyHostMemoryBarrier(const Ref<StagingBuffer>& srcBuffer, const PipelineStage& stage)
	{
		vk::BufferMemoryBarrier memBarrier{};

		vk::AccessFlagBits srcAccess = vk::AccessFlagBits::eHostWrite;
		vk::AccessFlagBits dstAccess = vk::AccessFlagBits::eTransferRead;

		switch (stage)
		{
		case PipelineStage::VertexShader: dstAccess = vk::AccessFlagBits::eShaderRead; break;
		case PipelineStage::FragmentShader: dstAccess = vk::AccessFlagBits::eShaderRead; break;
		case PipelineStage::ComputeShader:  dstAccess = vk::AccessFlagBits::eShaderRead; break;
		case PipelineStage::Transfer:  dstAccess = vk::AccessFlagBits::eTransferRead; break;
		default: ;
		}

		switch (stage)
		{
		case PipelineStage::VertexShader: srcAccess = vk::AccessFlagBits::eTransferWrite; break;
		case PipelineStage::FragmentShader: srcAccess = vk::AccessFlagBits::eTransferWrite; break;
		case PipelineStage::ComputeShader: srcAccess = vk::AccessFlagBits::eTransferWrite; break;
		case PipelineStage::Transfer:  srcAccess = vk::AccessFlagBits::eHostWrite; break;
		default: ;
		}

		memBarrier.setOffset(0);
		memBarrier.size = VK_WHOLE_SIZE;
		memBarrier.buffer = std::any_cast<vk::Buffer>(srcBuffer->GetHandle());
		memBarrier.srcAccessMask = vk::AccessFlagBits::eHostWrite;
		memBarrier.dstAccessMask = dstAccess;
		
		vk::PipelineStageFlagBits pipeStage =  vk::PipelineStageFlagBits::eTransfer;

		switch (stage)
		{
		case PipelineStage::VertexShader: pipeStage = vk::PipelineStageFlagBits::eVertexShader; break;
		case PipelineStage::FragmentShader: pipeStage = vk::PipelineStageFlagBits::eFragmentShader; break;
		case PipelineStage::ComputeShader: pipeStage = vk::PipelineStageFlagBits::eComputeShader; break;
		case PipelineStage::Transfer: pipeStage = vk::PipelineStageFlagBits::eTransfer;
		default: ;
		}


		m_CommandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eHost, pipeStage, {}, nullptr, { memBarrier }, nullptr);
	}

	void VulkanCommandBuffer::SetLineWidth(float lineWidth)
	{
		m_CommandBuffer.setLineWidth(lineWidth);
	}


	void VulkanCommandBuffer::CopyIndexBuffer(const Ref<StagingBuffer>& srcIndexBuffer,
	                                          const IndexBuffer* dstIndexBuffer)
	{
		CopyHostMemoryBarrier(srcIndexBuffer, PipelineStage::Transfer);

		vk::BufferCopy copyRegion{};
		copyRegion.dstOffset = 0;
		copyRegion.srcOffset = 0;

		if (srcIndexBuffer->GetSizeInBytes() != dstIndexBuffer->GetSizeInBytes())
		{
			__debugbreak();
		}

		copyRegion.size = dstIndexBuffer->GetSizeInBytes();
		const auto dstBuffer = std::any_cast<vk::Buffer>(dstIndexBuffer->GetHandle());
		const auto srcBuffer = std::any_cast<vk::Buffer>(srcIndexBuffer->GetHandle());

		m_CommandBuffer.copyBuffer(srcBuffer, dstBuffer, 1, &copyRegion);

	}

	void VulkanCommandBuffer::CopyVertexBuffer(const Ref<StagingBuffer>& srcVtxBuffer, const VertexBuffer* dstVtxBuffer)
	{
		CopyHostMemoryBarrier(srcVtxBuffer, PipelineStage::Transfer);
		vk::BufferCopy copyRegion{};
		copyRegion.dstOffset = 0;
		copyRegion.srcOffset = 0;

		if (srcVtxBuffer->GetSizeInBytes() != dstVtxBuffer->GetSizeInBytes())
		{
			__debugbreak();
		}

		copyRegion.size = dstVtxBuffer->GetSizeInBytes();
		const auto dstBuffer = std::any_cast<vk::Buffer>(dstVtxBuffer->GetHandle());
		const auto srcBuffer = std::any_cast<vk::Buffer>(srcVtxBuffer->GetHandle());

		m_CommandBuffer.copyBuffer(srcBuffer, dstBuffer, 1, &copyRegion);
	}

	void VulkanCommandBuffer::BindIndexBuffer(const Ref<IndexBuffer>& idxBuffer)
	{

		const auto buffer = std::any_cast<vk::Buffer>(idxBuffer->GetHandle());

		const auto idxType = idxBuffer->GetIndexDataType() == IndexDataType::UnsignedInt ? vk::IndexType::eUint32 : vk::IndexType::eUint16;

		m_CommandBuffer.bindIndexBuffer(buffer, 0, idxType);
	}

	void VulkanCommandBuffer::BindVertexBuffer(const Ref<VertexBuffer>& vtxBuffer)
	{

		auto buffer = std::any_cast<vk::Buffer>(vtxBuffer->GetHandle());
		m_CommandBuffer.bindVertexBuffers(0, { buffer }, { 0 });

	}

	void VulkanCommandBuffer::BindGraphicsPipeline(const Ref<GraphicsPipeline>& pipeline)
	{
		

		auto vulkanPipeline = std::any_cast<vk::Pipeline>(pipeline->GetHandle());
		m_CommandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, vulkanPipeline);
	}

#undef MemoryBarrier

	void VulkanCommandBuffer::CopyUniformBuffer(const Ref<StagingBuffer>& srcUbo, const Ref<UniformBuffer>& dstUbo, const PipelineStage& stage)
	{
		
		CopyHostMemoryBarrier(srcUbo, stage);
		const auto srcBuffer = std::any_cast<vk::Buffer>(srcUbo->GetHandle());
		const auto dstBuffer = std::any_cast<vk::Buffer>(dstUbo->GetHandle());

		vk::BufferCopy bufferCopy{};
		bufferCopy.size = srcUbo->GetSizeInBytes();
		bufferCopy.dstOffset = 0;
		bufferCopy.srcOffset = 0;

		m_CommandBuffer.copyBuffer(srcBuffer, dstBuffer, { bufferCopy });
		

	}

	void VulkanCommandBuffer::CopyStorageBuffer(const Ref<StagingBuffer>& srcUbo,
		const Ref<StorageBuffer>& storageBuffer, const PipelineStage& stage)
	{
		CopyHostMemoryBarrier(srcUbo, stage);

		auto srcBuffer = std::any_cast<vk::Buffer>(srcUbo->GetHandle());
		auto dstBuffer = std::any_cast<vk::Buffer>(storageBuffer->GetHandle());

		vk::BufferCopy bufferCopy{};
		bufferCopy.size = srcUbo->GetSizeInBytes();
		bufferCopy.dstOffset = 0;
		bufferCopy.srcOffset = 0;

		m_CommandBuffer.copyBuffer(srcBuffer, dstBuffer, { bufferCopy });
	}

	void VulkanCommandBuffer::SetViewPort(const Viewport& viewport)
	{
		vk::Viewport vkViewport{};
		vkViewport.height = viewport.Height;
		vkViewport.width = viewport.Width;
		vkViewport.x = viewport.X;
		vkViewport.y = viewport.Y;
		vkViewport.minDepth = viewport.MinDepth;
		vkViewport.maxDepth = viewport.MaxDepth;

		m_CommandBuffer.setViewport(0, 1, &vkViewport);
	}

	void VulkanCommandBuffer::SetScissor(const Rect& rect)
	{
		vk::Rect2D rect2d{};
		rect2d.extent.height = static_cast<uint32_t>(rect.Height);
		rect2d.extent.width = static_cast<uint32_t>(rect.Width);
		rect2d.offset.x = static_cast<int32_t>(rect.X);
		rect2d.offset.y = static_cast<int32_t>(rect.Y);

		m_CommandBuffer.setScissor(0, 1, &rect2d);
	}

	void VulkanCommandBuffer::BindGraphicsDescriptorSet(uint32_t setBinding, const Ref<PipelineDescriptorSet>& set)
	{
		const auto vulkanEngineSet = set.As<VulkanPipelineDescriptorSet>();
		const auto vulkanSet = std::any_cast<vk::DescriptorSet>(set->GetHandle());

		m_CommandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, vulkanEngineSet->GetDescLayout(), setBinding, {vulkanSet}, {});

	}

	void VulkanCommandBuffer::DrawIndexed(uint32_t count, const IndexDataType& type)
	{
		m_CommandBuffer.drawIndexed(count, 1, 0, 0, 0);
	}

	void VulkanCommandBuffer::DrawArrays(uint32_t count)
	{
		m_CommandBuffer.draw(count, 1, 0, 0);
	}

	void VulkanCommandBuffer::GraphicsPushConstant(const Ref<GraphicsPipeline>& pipeline, ShaderType type, const void* data, uint32_t size, uint32_t offset)
	{
		const auto vkPipeline = pipeline.As<VulkanGraphicsPipeline>();
		auto pipelineLayout = vkPipeline->GetPipelineLayout();

		vk::ShaderStageFlagBits stage{};

		switch (type)
		{
		case ShaderType::Vertex: stage = vk::ShaderStageFlagBits::eVertex; break;
		case ShaderType::Fragment: stage = vk::ShaderStageFlagBits::eFragment; break;
		case ShaderType::Compute: __debugbreak(); break;
		case ShaderType::None: break;
		default: ;
		}

		m_CommandBuffer.pushConstants(pipelineLayout, stage, offset, size, data);
	}

	void VulkanCommandBuffer::KomputePushConstant(const Ref<KomputePipeline>& pipeline, const void* data, uint32_t size,
		uint32_t offset)
	{
		const auto vkPipeline = pipeline.As<VulkanKomputePipeline>();
		auto pipelineLayout = vkPipeline->GetPipelineLayout();

		vk::ShaderStageFlagBits stage = vk::ShaderStageFlagBits::eCompute;


		m_CommandBuffer.pushConstants(pipelineLayout, stage, offset, size, data);
	}

	void VulkanCommandBuffer::Reset()
	{
		vk::Result result = m_CommandBuffer.reset();
		vk::resultCheck(result, "Failed to reset CommandBuffer");
	}

	void VulkanCommandBuffer::DispatchKompute(const glm::uvec3& workGroup)
	{
		m_CommandBuffer.dispatch(workGroup.x, workGroup.y, workGroup.z);
	}

	void VulkanCommandBuffer::BindKomputeDescriptorSet(uint32_t setBinding, const Ref<PipelineDescriptorSet>& set)
	{
		const auto vulkanEngineSet = set.As<VulkanPipelineDescriptorSet>();
		const auto vulkanSet = std::any_cast<vk::DescriptorSet>(set->GetHandle());

		m_CommandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eCompute, vulkanEngineSet->GetDescLayout(), setBinding, { vulkanSet }, {});
	}

	void VulkanCommandBuffer::BindKomputePipline(const Ref<KomputePipeline>& pipeline)
	{
		auto vulkanPipeline = std::any_cast<vk::Pipeline>(pipeline->GetHandle());
		m_CommandBuffer.bindPipeline(vk::PipelineBindPoint::eCompute, vulkanPipeline);
	}


	std::any VulkanCommandBuffer::GetHandle()
	{
		return m_CommandBuffer;
	}

	VulkanCommandBuffer::~VulkanCommandBuffer()
	{
	}
}
