#pragma once
#include "Engine/Renderer/CommandList.h"
#include "vulkan/vulkan.hpp"

namespace Polyboid
{
	class VulkanCommandBuffer;
	class VkRenderAPI;

	class VulkanCommandList : public CommandList
	{
	private:

		const VkRenderAPI* m_Context;
		vk::CommandPool m_CommandList;
		std::vector<Ref<VulkanCommandBuffer>> m_CommandBuffers;

	public:

		void Destroy(vk::Device device);

		VulkanCommandList(const VkRenderAPI* context);
		Ref<CommandBuffer> CreateCommandBuffer(uint32_t count) override;

		void WaitAndRender() override;
		~VulkanCommandList() override;

		friend class VulkanCommandBuffer;
	};
}


