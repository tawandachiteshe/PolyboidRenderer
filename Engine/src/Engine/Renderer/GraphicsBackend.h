#pragma once
#include <functional>

#include "Engine/Engine/Base.h"


namespace vk
{
	class CommandBuffer;
}

namespace Polyboid
{
	class CommandBuffer;
	class CommandBufferSet;
	class FrameBufferSet;
	class RenderPass;
	class PipelineState;

	class GraphicsBackend
	{
	public:

		using RenderBackendFreeFunc = std::function<void()>;

		virtual void RegisterResizeFunc(const RenderBackendFreeFunc& freeFunc) = 0;
		virtual void SubmitPipeline(const Ref<PipelineState>& pipeline) = 0;
		virtual void SubmitRenderpass(const Ref<RenderPass>& renderpass) = 0;
		virtual void SubmitFramebuffer(const Ref<FrameBufferSet>& framebuffer) = 0;
		virtual void GetSwapchainImageIndex(uint32_t frameIndex) = 0;
		virtual bool IsReady() = 0;
		virtual void SubmitGraphicsWork(const std::vector<Ref<CommandBufferSet>>& commandBuffer) = 0;
		virtual void SubmitOneTimeWork(const Ref<CommandBuffer>& buffer) = 0;
		virtual uint32_t GetCurrentImageIndex() = 0;

		virtual ~GraphicsBackend() {}
	};

	
}

