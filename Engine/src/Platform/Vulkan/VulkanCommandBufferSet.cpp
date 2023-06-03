#include "boidpch.h"

#include  <vulkan/vulkan.hpp>

#include "VulkanCommandBufferSet.h"
#include <spdlog/spdlog.h>
#include "VkRenderAPI.h"
#include "VulkanCommandBuffer.h"
#include "Engine/Renderer/Renderer.h"
#include "Utils/VulkanDevice.h"
#include "Utils/VulkanPhysicalDevice.h"

namespace Polyboid
{
	void VulkanCommandBufferSet::Destroy(vk::Device device)
	{
		for (const auto& commandBuffer : m_CommandBuffers)
		{
			commandBuffer->Destroy(device);
		}

		if (m_CommandList)
		{
			device.destroyCommandPool(m_CommandList);
			m_CommandList = nullptr;
		}

		m_CommandBuffers.clear();
		
	}

	VulkanCommandBufferSet::VulkanCommandBufferSet(const VkRenderAPI* context, const CommandListSettings& settings): m_Context(context), m_Settings(settings)
	{

		Init(context, m_Settings);

	}

	void VulkanCommandBufferSet::Init(const VkRenderAPI* context, const CommandListSettings& settings)
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

	void VulkanCommandBufferSet::Recreate()
	{
		for (const auto& commandBuffer : m_CommandBuffers)
		{
			commandBuffer->Destroy(VkRenderAPI::GetVulkanDevice());
		}

		Destroy(VkRenderAPI::GetVulkanDevice());
		Init(m_Context, m_Settings);
	}

	CommandListSettings& VulkanCommandBufferSet::GetSettings()
	{
		return m_Settings;
	}


	Ref<CommandBuffer> VulkanCommandBufferSet::GetCommandBufferAt(uint32_t index)
	{
		return m_CommandBuffers.at(index).As<CommandBuffer>();
	}


	VulkanCommandBufferSet::~VulkanCommandBufferSet()
	{
		
	}
}
