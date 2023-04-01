#include "boidpch.h"
#include "VulkanCommandBuffer.h"

#include <spdlog/spdlog.h>

#include "imgui.h"
#include "imgui_impl_vulkan.h"
#include "VkRenderAPI.h"
#include "VkSwapChain.h"
#include "VulkanCommandList.h"
#include "VulkanFramebuffer.h"
#include "VulkanRenderPass.h"
#include "Engine/Engine/Application.h"
#include "Engine/Engine/ImguiSetup.h"
#include "vulkan/vulkan.hpp"
#include "Utils/VulkanDevice.h"

namespace Polyboid
{

	VulkanCommandBuffer::VulkanCommandBuffer(const VkRenderAPI* context, const VulkanCommandList* commands): m_Context(context), m_CommandList(commands)
	{

		m_CommandBuffer.resize(m_Context->MAX_FRAMES_INFLIGHT);

		vk::Device device = *context->GetDevice();

		vk::CommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = vk::StructureType::eCommandBufferAllocateInfo;
		allocInfo.commandPool = commands->m_CommandList;
		allocInfo.level = vk::CommandBufferLevel::ePrimary;
		allocInfo.commandBufferCount = static_cast<uint32_t>(m_CommandBuffer.size());

		auto [result, commandbuffer] = device.allocateCommandBuffers(allocInfo);
		vk::resultCheck(result, "Failed to  alloc command buffer");
		m_CommandBuffer = commandbuffer;

	}

	void VulkanCommandBuffer::Begin()
	{
		vk::Device device = *m_Context->GetDevice();

		vk::Result result = m_CommandBuffer[m_Context->m_CurrentFrame].reset();
		vk::resultCheck(result, "Failed to reset command buffer");

		vk::CommandBufferBeginInfo beginInfo{};
		beginInfo.sType = vk::StructureType::eCommandBufferBeginInfo;
		beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
		beginInfo.pInheritanceInfo = nullptr;

		result = m_CommandBuffer[m_Context->m_CurrentFrame].begin(beginInfo);
		vk::resultCheck(result, "Failed to begin command buffer");

		
	}

	void VulkanCommandBuffer::End()
	{
		vk::CommandBuffer::NativeType cmd = m_CommandBuffer[m_Context->m_CurrentFrame];
		

		 auto result = m_CommandBuffer[m_Context->m_CurrentFrame].end();
		 vk::resultCheck(result, "Failed to end command buffer");
	}

	void VulkanCommandBuffer::BeginRenderPass(const Ref<RenderPass>& renderPass)
	{

		
		auto vkRenderpass = std::reinterpret_pointer_cast<VulkanRenderPass>(renderPass);
		vk::RenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = vk::StructureType::eRenderPassBeginInfo;

		if (vkRenderpass->m_Settings.type == RenderPassType::Present)
		{
			auto vkSwapchain = std::reinterpret_pointer_cast<VkSwapChain>(Application::Get().GetSwapchain());
			renderPassInfo.renderPass = vkRenderpass->m_RenderPass;
			renderPassInfo.framebuffer = vkSwapchain->GetSwapbuffers()[m_Context->m_SwapchainImageIndex]->m_Framebuffer;
		}
		else
		{
			auto vkFramebuffer = std::reinterpret_pointer_cast<VulkanFramebuffer>(renderPass->GetFramebuffer());
			renderPassInfo.renderPass = vkRenderpass->m_RenderPass;
			renderPassInfo.framebuffer = vkFramebuffer->m_Framebuffer;
		}

		renderPassInfo.renderArea.offset = vk::Offset2D{0, 0};
		renderPassInfo.renderArea.extent = vk::Extent2D{ vkRenderpass->m_Settings.Width, vkRenderpass->m_Settings.Height };
		vk::ClearValue clearValue;

		auto clearSettings = vkRenderpass->GetClearSettings();

		clearValue.color.float32[0] = clearSettings.color.x;
		clearValue.color.float32[1] = clearSettings.color.y;
		clearValue.color.float32[2] = clearSettings.color.z;
		clearValue.color.float32[3] = clearSettings.color.w;
		

		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearValue;

		m_CommandBuffer[m_Context->m_CurrentFrame].beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
		
	}

	void VulkanCommandBuffer::EndRenderPass()
	{
		m_CommandBuffer[m_Context->m_CurrentFrame].endRenderPass();
	}

	void VulkanCommandBuffer::WaitAndRender()
	{
		vk::SubmitInfo submitInfo{};
		auto imageSemaphore = m_Context->GetSyncObjects()->GetImageSemaphores()[m_Context->m_CurrentFrame];
		auto fence = m_Context->GetSyncObjects()->GetFences()[m_Context->m_CurrentFrame];
		auto renderSemaphore = m_Context->GetSyncObjects()->GetRenderSemaphores()[m_Context->m_CurrentFrame];
		auto graphicsQueue = m_Context->GetDevice()->GetGraphicsQueue();

		Imgui::ImguiData& data = Imgui::GetData();

		const vk::CommandBuffer buffers[] = {m_CommandBuffer[m_Context->m_CurrentFrame], data.m_ImguiCommandBuffer->GetCommandBuffers()[m_Context->m_CurrentFrame]};

		vk::PipelineStageFlags waitStage = vk::PipelineStageFlagBits::eColorAttachmentOutput;
		submitInfo.pWaitSemaphores = &imageSemaphore;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitDstStageMask = &waitStage;
		submitInfo.commandBufferCount = 2;
		submitInfo.pCommandBuffers = buffers;
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &renderSemaphore;


		auto result = graphicsQueue.submit(1, &submitInfo, fence);

		if (result != vk::Result::eSuccess)
		{
			spdlog::error("unable to submit commands");
			__debugbreak();
		}

	}

	VulkanCommandBuffer::~VulkanCommandBuffer()
	{
	}
}
