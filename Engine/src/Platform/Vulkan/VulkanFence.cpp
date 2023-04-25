#include "boidpch.h"
#include "VulkanFence.h"

#include "VkRenderAPI.h"
#include "Utils/VulkanDevice.h"


namespace Polyboid
{
	VulkanFence::VulkanFence(const VkRenderAPI* context): m_Context(context)
	{
		
		vk::Device device = (*context->GetDevice());
		m_VulkanDevice = device;
		vk::FenceCreateInfo fenceCreateInfo{};
		fenceCreateInfo.flags = vk::FenceCreateFlagBits::eSignaled;

		auto [result, _fence] = device.createFence(fenceCreateInfo);

		vk::resultCheck(result, "Failed to create fence");
		

		m_Handle = _fence;

	}

	void VulkanFence::WaitAndReset()
	{
		vk::Result result = m_VulkanDevice.waitForFences(1, &m_Handle, true, std::numeric_limits<uint64_t>::max());
		vk::resultCheck(result, "Failed to wait for fence");

		result = m_VulkanDevice.resetFences(1, &m_Handle);
		vk::resultCheck(result, "Failed to wait for fence");

	}

	void VulkanFence::Destroy()
	{
		vk::Device device = (*m_Context->GetDevice());
		device.destroyFence(m_Handle);
	}

	std::any VulkanFence::GetHandle()
	{
		return m_Handle;
	}

	VulkanFence::~VulkanFence()
	{
	}
}
