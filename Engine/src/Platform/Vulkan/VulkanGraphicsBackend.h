#pragma once
#include <functional>
#include <vulkan/vulkan.hpp>

#include "Engine/Engine/Base.h"
#include "Engine/Renderer/GraphicsBackend.h"


namespace Polyboid
{
	class Swapchain;
	class RenderPass;
	class GraphicsPipeline;
	class VulkanPipelineDescriptorSetPool;
	class CommandBufferSet;
	class GraphicsSyncObjects;
	class UniformBufferSet;
	class StagingBufferSet;
	class StorageBufferSet;
	class VulkanVertexBufferArray;
	class FrameBufferSet;
	class VulkanStagingBuffer;
	class VulkanUniformBuffer;
	class VulkanShaderStorage;
	class VulkanGraphicsPipeline;
	class VulkanImage2D;
	class VulkanSemaphore;
	class VulkanFence;
	class VulkanVertexBuffer;
	class VulkanIndexBuffer;
	class VulkanTexture2D;
	class VulkanCommandBufferSet;
	class VulkanFramebuffer;
	class VulkanRenderPass;
	class VkSwapChain;

	struct GraphicsBackendData
	{
		Ref<Swapchain> m_Swapchain;
		std::vector<Ref<VulkanRenderPass>> m_RenderPasses;
		std::vector<Ref<FrameBufferSet>> m_Framebuffers;
		std::vector<Ref<VulkanGraphicsPipeline>> m_Pipelines;
		std::vector<Ref<CommandBufferSet>> m_CommandBuffers;

		std::vector<vk::CommandBuffer> m_SubmittingBuffer;
		Ref<GraphicsSyncObjects> m_SyncObjects;
	};

	class VulkanGraphicsBackend : public GraphicsBackend
	{
	public:
		
	private:
		Ref<GraphicsBackendData> s_Data;
		uint32_t m_SwapchainIndex = 0;
		vk::Queue m_GraphicsQueue;
		vk::Device m_Device;
		bool m_CanRender = true;
		std::vector<RenderBackendFreeFunc> m_FreeFunctions;
		std::vector<vk::Semaphore> m_WaitSemaphores;

		
		void Init();
		void ClearResources();

	public:

		
		 
		VulkanGraphicsBackend();
		void RecreateResources();
		void RegisterResizeFunc(const RenderBackendFreeFunc& freeFunc) override;

		void SubmitPipeline(const Ref<GraphicsPipeline>& pipeline) override;
		void SubmitRenderpass(const Ref<RenderPass>& renderpass) override;
		void SubmitFramebuffer(const Ref<FrameBufferSet>& framebuffer) override;
		void GetSwapchainImageIndex(uint32_t frameIndex) override;
		bool IsReady() override;

		void Present() override;
		void SubmitGraphicsWork(const std::vector<Ref<CommandBufferSet>>& commandBuffer) override;
		void SubmitOneTimeWork(const Ref<CommandBuffer>& buffer) override;
		uint32_t GetCurrentImageIndex() override;
		void WaitForQueue() override;
		~VulkanGraphicsBackend() override;
	};

}
