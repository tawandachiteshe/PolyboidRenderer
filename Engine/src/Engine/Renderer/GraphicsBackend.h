#pragma once
#include <functional>

#include "Engine/Engine/Base.h"


namespace vk
{
	class CommandBuffer;
}

namespace Polyboid
{
	class ComputeSyncObjects;
	class CommandBuffer;
	class CommandBufferSet;
	class FrameBufferSet;
	class RenderPass;
	class GraphicsPipeline;

	class GraphicsBackend
	{
	public:

		using RenderBackendFreeFunc = std::function<void()>;

		virtual void RegisterResizeFunc(const RenderBackendFreeFunc& freeFunc) = 0;
		virtual void SubmitPipeline(const Ref<GraphicsPipeline>& pipeline) = 0;
		virtual void SubmitRenderpass(const Ref<RenderPass>& renderpass) = 0;
		virtual void SubmitFramebuffer(const Ref<FrameBufferSet>& framebuffer) = 0;
		virtual void GetSwapchainImageIndex(uint32_t frameIndex) = 0;
		virtual bool IsReady() = 0;
		virtual void SubmitGraphicsWork(const std::vector<Ref<CommandBufferSet>>& commandBuffer) = 0;
		virtual void SubmitOneTimeWork(const Ref<CommandBuffer>& buffer) = 0;
		virtual void Present() = 0;
		virtual uint32_t GetCurrentImageIndex() = 0;
		virtual void WaitForQueue() = 0;

		virtual ~GraphicsBackend() = default;
	};

	class ComputeBackend
	{
	public:
		virtual void WaitAndResetFence() = 0;
		virtual Ref<ComputeSyncObjects> GetSyncObjects() = 0;
		virtual void SubmitComputeWork(const std::vector<Ref<CommandBufferSet>>& commandBuffer) = 0;
		virtual void ComputeOneTime(const std::vector<Ref<CommandBufferSet>>& commandBuffer, uint32_t frameIndex) = 0;
		virtual ~ComputeBackend();
	};

	
}

