#include "boidpch.h"
#include "VulkanFence.h"

#include <spdlog/spdlog.h>

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

		vk::Fence::NativeType fenceId = _fence;
		spdlog::info("Fence id {}", (uint64_t)fenceId);
		

		m_Handle = _fence;

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
