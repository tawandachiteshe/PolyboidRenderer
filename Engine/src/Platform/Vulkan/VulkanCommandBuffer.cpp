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
#include "VulkanRenderPass.h"
#include "VulkanSemaphore.h"
#include "Engine/Engine/Application.h"
#include "Engine/Engine/ImguiSetup.h"
#include "vulkan/vulkan.hpp"
#include "Utils/VulkanDevice.h"

namespace Polyboid
{
	void VulkanCommandBuffer::Destroy(const vk::Device& device)
	{
		m_Fence->Destroy();
		m_ImageSemaphore->Destroy();
		m_RenderSemaphore->Destroy();
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

		m_Fence = std::make_shared<VulkanFence>(m_Context);
		m_ImageSemaphore = std::make_shared<VulkanSemaphore>(m_Context);
		m_RenderSemaphore = std::make_shared<VulkanSemaphore>(m_Context);


	}

	void VulkanCommandBuffer::Begin()
	{
		vk::Device device = *m_Context->GetDevice();


		auto inflightFence = std::any_cast<vk::Fence>(m_Fence->GetHandle());


		vk::Result result = device.waitForFences(1, &inflightFence, true, std::numeric_limits<uint64_t>::max());
		

		if (result != vk::Result::eSuccess)
		{
			spdlog::error("Waiting for fence failed");
			__debugbreak();
		}


		result = m_CommandBuffer.reset();
		vk::resultCheck(result, "Failed to reset command buffer");

		vk::CommandBufferBeginInfo beginInfo{};
		beginInfo.sType = vk::StructureType::eCommandBufferBeginInfo;
		beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
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

		auto vkRenderpass = std::reinterpret_pointer_cast<VulkanRenderPass>(renderPass);
		const vk::RenderPassBeginInfo renderPassInfo = vkRenderpass->GetRenderBeginInfo(m_SwapchainCurrentImageIndex);
		m_CommandBuffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
		
	}

	void VulkanCommandBuffer::EndRenderPass()
	{
		m_CommandBuffer.endRenderPass();
	}

	void VulkanCommandBuffer::SubmitAndRender()
	{
		vk::SubmitInfo submitInfo{};
		auto imageSemaphore = std::any_cast<vk::Semaphore>(m_ImageSemaphore->GetHandle());
		auto fence = std::any_cast<vk::Fence>(m_Fence->GetHandle());
		auto renderSemaphore = std::any_cast<vk::Semaphore>(m_RenderSemaphore->GetHandle());
		auto graphicsQueue = m_CommandList->m_GraphicsQueue;


		vk::PipelineStageFlags waitStage = vk::PipelineStageFlagBits::eColorAttachmentOutput;
		submitInfo.pWaitSemaphores = &imageSemaphore;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitDstStageMask = &waitStage;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_CommandBuffer;
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &renderSemaphore;


		auto result = graphicsQueue.submit(1, &submitInfo, fence);

		if (result != vk::Result::eSuccess)
		{
			spdlog::error("unable to submit commands");
			__debugbreak();
		}

	}

	void VulkanCommandBuffer::SubmitSwapchain(const Ref<Swapchain>& swapchain)
	{
		m_Swapchain = std::reinterpret_pointer_cast<VkSwapChain>(swapchain);

		vk::Device device = *m_Context->GetDevice();
		auto imageSemaphore = std::any_cast<vk::Semaphore>(m_ImageSemaphore->GetHandle());

		auto ImageResult = device.acquireNextImageKHR(m_Swapchain->GetSwapchain(), std::numeric_limits<uint64_t>::max(), imageSemaphore);
		m_SwapchainCurrentImageIndex = ImageResult.value;

		if (ImageResult.result == vk::Result::eErrorOutOfDateKHR)
		{
			m_Swapchain->Invalidate();
			return;
		}

		if (ImageResult.result != vk::Result::eSuccess || ImageResult.result == vk::Result::eSuboptimalKHR)
		{
			spdlog::error("Swapchain not great??");
			__debugbreak();
		}

		auto fence = std::any_cast<vk::Fence>(m_Fence->GetHandle());
		auto result = device.resetFences(1, &fence);
		if (result != vk::Result::eSuccess)
		{
			spdlog::error("Reset fence failed");
			__debugbreak();
		}
	}

	void VulkanCommandBuffer::SubmitAndPresent()
	{

		auto renderSemaphore = std::any_cast<vk::Semaphore>(m_RenderSemaphore->GetHandle());
		uint32_t imageIndex = m_SwapchainCurrentImageIndex;
		vk::Result presentResult = vk::Result::eSuccess;

		auto swapChain = m_Swapchain->GetSwapchain();


		vk::PresentInfoKHR presentInfo{};
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &renderSemaphore;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pSwapchains = &swapChain;
		presentInfo.pResults = &presentResult;
		presentInfo.swapchainCount = 1;

		auto result = m_CommandList->m_PresentQueue.presentKHR(&presentInfo);


		if (presentResult == vk::Result::eErrorOutOfDateKHR) {

			m_Swapchain->Invalidate();
		}
		else if (result != vk::Result::eSuccess && result == vk::Result::eSuboptimalKHR)
		{
			spdlog::error("Failed to present");
			__debugbreak();
		}

	}

	std::any VulkanCommandBuffer::GetHandle()
	{
		return m_CommandBuffer;
	}

	VulkanCommandBuffer::~VulkanCommandBuffer()
	{
	}
}
