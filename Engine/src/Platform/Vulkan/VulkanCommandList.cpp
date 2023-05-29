#include "boidpch.h"

#include  <vulkan/vulkan.hpp>

#include "VulkanCommandList.h"
#include <spdlog/spdlog.h>
#include "VkRenderAPI.h"
#include "VulkanCommandBuffer.h"
#include "Engine/Renderer/Renderer.h"
#include "Utils/VulkanDevice.h"
#include "Utils/VulkanPhysicalDevice.h"

namespace Polyboid
{
	void VulkanCommandList::Destroy(vk::Device device)
	{
		for (auto& commandBuffer : m_CommandBuffers)
		{
			commandBuffer->Destroy(device);
		}

		if (m_CommandList)
		{
			device.destroyCommandPool(m_CommandList);
			m_CommandList = nullptr;
		}
		
	}

	VulkanCommandList::VulkanCommandList(const VkRenderAPI* context, const CommandListSettings& settings): m_Context(context)
	{

		vk::Device device = *context->GetDevice();
		auto indices = context->GetPhysicalDevice()->GetFamilyIndices();


		//TODO: make this more configurable
		vk::CommandPoolCreateInfo createInfo;
		createInfo.sType = vk::StructureType::eCommandPoolCreateInfo;
		createInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
		createInfo.queueFamilyIndex = indices.GraphicsFamily.value();
		auto [result, commandList] = device.createCommandPool(createInfo);
		vk::resultCheck(result, "Failed to create command pool");
		m_CommandList = commandList;

		m_GraphicsQueue = context->GetDevice()->GetGraphicsQueue();
		m_PresentQueue = context->GetDevice()->GetPresentQueue();

		for (uint32_t i = 0; i < settings.ImageCount; ++i)
		{
			auto vulkanCommandBuffer = CreateRef<VulkanCommandBuffer>(m_Context, this);
			m_CommandBuffers.push_back(vulkanCommandBuffer);
		}


	}



	Ref<CommandBuffer> VulkanCommandList::GetCommandBufferAt(uint32_t index)
	{
		return m_CommandBuffers.at(index).As<CommandBuffer>();
	}


	VulkanCommandList::~VulkanCommandList()
	{
		
	}
}
