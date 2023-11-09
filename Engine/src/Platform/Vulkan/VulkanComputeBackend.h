#pragma once
#include "Engine/Renderer/GraphicsBackend.h"
#include "Utils/VulkanDevice.h"


namespace Polyboid
{
	class ComputeSyncObjects;

	class VulkanComputeBackend : public ComputeBackend
	{

	private:
		vk::Queue m_GraphicsQueue;
		vk::Device m_Device;
		Ref<ComputeSyncObjects> m_SyncObjects;
		std::vector<Ref<CommandBufferSet>> m_CommandBuffers;
		std::vector<vk::CommandBuffer> m_SubmittingBuffers;
	public:
		VulkanComputeBackend();
		void WaitAndResetFence() override;
		Ref<ComputeSyncObjects> GetSyncObjects() override;

		bool CanAddWaitSemaphore() const { return !m_SubmittingBuffers.empty(); }

		void SubmitComputeWork(const std::vector<Ref<CommandBufferSet>>& commandBuffer) override;
		void ComputeOneTime(const std::vector<Ref<CommandBufferSet>>& commandBuffer, uint32_t frameIndex) override;
		~VulkanComputeBackend() override;
	};

}

