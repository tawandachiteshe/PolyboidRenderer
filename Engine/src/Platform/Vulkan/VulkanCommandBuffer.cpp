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

	void VulkanCommandBuffer::BeginRenderPass(const Ref<RenderPass>& renderPass, const Ref<Framebuffer>& framebuffer)
	{

		auto vkRenderpass = std::reinterpret_pointer_cast<VulkanRenderPass>(renderPass);
		auto vkFramebuffer = std::reinterpret_pointer_cast<VulkanFramebuffer>(framebuffer);
		const vk::RenderPassBeginInfo renderPassInfo = vkRenderpass->GetRenderBeginInfo(vkFramebuffer);
		m_CommandBuffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
		
	}

	void VulkanCommandBuffer::EndRenderPass()
	{
		
		m_CommandBuffer.endRenderPass();
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
