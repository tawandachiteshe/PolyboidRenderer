#include "boidpch.h"
#include "VulkanComputeBackend.h"

#include "VkRenderAPI.h"
#include "Engine/Renderer/CommandBufferSet.h"
#include "Engine/Renderer/GraphicsSyncObjects.h"
#include "Engine/Renderer/KomputeCommand.h"
#include "Engine/Renderer/RenderCommand.h"
#include "Engine/Renderer/SyncObjects.h"


namespace Polyboid
{
	VulkanComputeBackend::VulkanComputeBackend()
	{
		m_Device = VkRenderAPI::GetVulkanDevice();
		m_GraphicsQueue = VkRenderAPI::GetVulkanQueue();
		m_SyncObjects = ComputeSyncObjects::Create();
	}

	void VulkanComputeBackend::WaitAndResetFence()
	{
		const auto fence = std::any_cast<vk::Fence>(
			m_SyncObjects->GetInFlightFence(RenderCommand::GetCurrentFrame())->GetHandle());

		auto result = m_Device.waitForFences({fence}, VK_TRUE, UINT64_MAX);
		vk::resultCheck(result, "Failed to reset compute fences");

		result = m_Device.resetFences({fence});
		vk::resultCheck(result, "Failed to reset fences");
	}

	Ref<ComputeSyncObjects> VulkanComputeBackend::GetSyncObjects()
	{
		return m_SyncObjects;
	}

	void VulkanComputeBackend::SubmitComputeWork(const std::vector<Ref<CommandBufferSet>>& commandBuffer)
	{
		m_CommandBuffers = commandBuffer;


		const auto fence = std::any_cast<vk::Fence>(
			m_SyncObjects->GetInFlightFence(RenderCommand::GetCurrentFrame())->GetHandle());

		for (const auto& commandSet : m_CommandBuffers)
		{
			vk::CommandBuffer buffer = std::any_cast<vk::CommandBuffer>(
				commandSet->GetCommandBufferAt(RenderCommand::GetCurrentFrame())->GetHandle());
			m_SubmittingBuffers.emplace_back(buffer);
		}


		vk::SubmitInfo submitInfo{};
		submitInfo.setCommandBuffers(m_SubmittingBuffers);


		const auto result = m_GraphicsQueue.submit({submitInfo}, {fence});
		vk::resultCheck(result, "Failed to submit work");

		m_SubmittingBuffers.clear();;
	}

	void VulkanComputeBackend::ComputeOneTime(const std::vector<Ref<CommandBufferSet>>& commandBuffer,
	                                          uint32_t frameIndex)
	{
		for (const auto& commandSet : commandBuffer)
		{
			vk::CommandBuffer buffer = std::any_cast<vk::CommandBuffer>(
				commandSet->GetCommandBufferAt(frameIndex)->GetHandle());
			m_SubmittingBuffers.emplace_back(buffer);
		}


		vk::SubmitInfo submitInfo{};
		submitInfo.setCommandBuffers(m_SubmittingBuffers);

		auto result = m_GraphicsQueue.submit({submitInfo}, {});
		vk::resultCheck(result, "Failed to submit work");
		result = m_GraphicsQueue.waitIdle();
		vk::resultCheck(result, "Failed to wait");

		m_SubmittingBuffers.clear();
	}

	VulkanComputeBackend::~VulkanComputeBackend()
	{
	}
}
