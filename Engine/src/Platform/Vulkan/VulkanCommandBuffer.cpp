#include "boidpch.h"
#include "VulkanCommandBuffer.h"

#include <spdlog/spdlog.h>

#include "imgui.h"
#include "imgui_impl_vulkan.h"
#include "VkRenderAPI.h"
#include "VkSwapChain.h"
#include "VulkanCommandList.h"
#include "VulkanFence.h"
#include "VulkanFramebuffer.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanPipelineDescriptorSet.h"
#include "VulkanRenderPass.h"
#include "VulkanSemaphore.h"
#include "Engine/Engine/Application.h"
#include "Engine/Engine/ImguiSetup.h"
#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/Image2D.h"
#include "Engine/Renderer/PipelineDescriptorSet.h"
#include "Engine/Renderer/PipelineState.h"
#include "vulkan/vulkan.hpp"
#include "Utils/VulkanDevice.h"
#include "Utils/Common.h"

namespace Polyboid
{
	void VulkanCommandBuffer::Destroy(const vk::Device& device)
	{
		
	}

	VulkanCommandBuffer::VulkanCommandBuffer(const VkRenderAPI* context, const VulkanCommandList* commands): m_Context(context), m_CommandList(commands)
	{


		vk::Device device = *context->GetDevice();

		vk::CommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = vk::StructureType::eCommandBufferAllocateInfo;
		allocInfo.commandPool = commands->m_CommandList;
		allocInfo.level = vk::CommandBufferLevel::ePrimary;
		allocInfo.commandBufferCount = 1;

		auto [result, commandbuffer] = device.allocateCommandBuffers(allocInfo);
		vk::resultCheck(result, "Failed to  alloc command buffer");
		m_CommandBuffer = commandbuffer.at(0);



	}

	void VulkanCommandBuffer::Begin()
	{

		vk::Result result = m_CommandBuffer.reset();
		vk::resultCheck(result, "Failed to reset command buffer");

		vk::CommandBufferBeginInfo beginInfo{};
		beginInfo.sType = vk::StructureType::eCommandBufferBeginInfo;
		//beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
		beginInfo.pInheritanceInfo = nullptr;
		
		 result = m_CommandBuffer.begin(beginInfo);
		vk::resultCheck(result, "Failed to begin command buffer");

	}

	void VulkanCommandBuffer::End()
	{
		 auto result = m_CommandBuffer.end();
		 vk::resultCheck(result, "Failed to end command buffer");
	}

	void VulkanCommandBuffer::BeginRenderPass(const Ref<RenderPass>& renderPass, const Ref<Framebuffer>& framebuffer)
	{

		auto vkRenderpass = std::reinterpret_pointer_cast<VulkanRenderPass>(renderPass);
		auto vkFramebuffer = std::reinterpret_pointer_cast<VulkanFramebuffer>(framebuffer);
		const vk::RenderPassBeginInfo renderPassInfo = vkRenderpass->GetRenderBeginInfo(vkFramebuffer);
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

	void VulkanCommandBuffer::TransitionImageLayout(const Ref<Image2D>& src, ImageLayout newLayout)
	{

		vk::ImageMemoryBarrier barrier{};
		barrier.oldLayout = Utils::ConvertToVulkanLayout(src->GetLayout());
		barrier.newLayout = Utils::ConvertToVulkanLayout(newLayout);

		barrier.srcQueueFamilyIndex = 0;
		barrier.dstQueueFamilyIndex = 0;
		barrier.image = std::any_cast<vk::Image>(src->GetHandle());
		barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		barrier.srcAccessMask = vk::AccessFlagBits::eNone; //TODO: important wangu
		barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite; //TODO: important wangu

		vk::PipelineStageFlags dstStageFlags = vk::PipelineStageFlagBits::eTransfer; //TODO: importang again we need to know;
		vk::PipelineStageFlags srcStageFlags = vk::PipelineStageFlagBits::eTopOfPipe; //TODO: importang again we need to know;

		vk::DependencyFlags dependencyFlags = static_cast<vk::DependencyFlags>(0);

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

	void VulkanCommandBuffer::CopyIndexBuffer(const Ref<StagingBuffer>& srcIndexBuffer,
		const IndexBuffer* dstIndexBuffer)
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

	void VulkanCommandBuffer::CopyVertexBuffer(const Ref<StagingBuffer>& srcVtxBuffer, const VertexBuffer* dstVtxBuffer)
	{
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

		auto buffer = std::any_cast<vk::Buffer>(idxBuffer->GetHandle());

		auto idxType = idxBuffer->GetIndexDataType() == IndexDataType::UnsignedInt ? vk::IndexType::eUint32 : vk::IndexType::eUint16;

		m_CommandBuffer.bindIndexBuffer(buffer, 0, idxType);
	}

	void VulkanCommandBuffer::BindVertexBuffer(const Ref<VertexBuffer>& vtxBuffer)
	{

		auto buffer = std::any_cast<vk::Buffer>(vtxBuffer->GetHandle());
		m_CommandBuffer.bindVertexBuffers(0, { buffer }, { 0 });

	}

	void VulkanCommandBuffer::BindGraphicsPipeline(const Ref<PipelineState>& pipeline)
	{
		

		auto vulkanPipeline = std::any_cast<vk::Pipeline>(pipeline->GetHandle());
		m_CommandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, vulkanPipeline);
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

	void VulkanCommandBuffer::BindDescriptorSet(uint32_t setBinding, const Ref<PipelineDescriptorSet>& set)
	{
		const auto vulkanEngineSet = std::reinterpret_pointer_cast<VulkanPipelineDescriptorSet> (set);
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

	void VulkanCommandBuffer::Reset()
	{
		vk::Result result = m_CommandBuffer.reset();
		vk::resultCheck(result, "Failed to reset CommandBuffer");
	}


	std::any VulkanCommandBuffer::GetHandle()
	{
		return m_CommandBuffer;
	}

	VulkanCommandBuffer::~VulkanCommandBuffer()
	{
	}
}
