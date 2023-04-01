#include "boidpch.h"
#include "VulkanCommandList.h"

#include "VkRenderAPI.h"
#include "VulkanCommandBuffer.h"
#include "Utils/VulkanDevice.h"
#include "Utils/VulkanPhysicalDevice.h"

namespace Polyboid
{
	void VulkanCommandList::Destroy(vk::Device device)
	{

		device.destroyCommandPool(m_CommandList);
	}

	VulkanCommandList::VulkanCommandList(const VkRenderAPI* context): m_Context(context)
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

	}

	Ref<CommandBuffer> VulkanCommandList::CreateCommandBuffer(uint32_t count)
	{
		auto vulkanCommandBuffer = std::make_shared<VulkanCommandBuffer>(m_Context, this);
		m_CommandBuffers.push_back(vulkanCommandBuffer);
		return vulkanCommandBuffer;
	}


	void VulkanCommandList::WaitAndRender()
	{
	}

	VulkanCommandList::~VulkanCommandList()
	{
		
	}
}
