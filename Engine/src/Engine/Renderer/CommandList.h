#pragma once
#include <cstdint>

#include "Engine/Engine/Base.h"

namespace Polyboid
{
	class Framebuffer;
	class Swapchain;
	class RenderPass;

	class CommandBuffer
	{
	public:


		virtual void Begin() = 0;
		virtual void End() = 0;
		virtual void BeginRenderPass(const Ref<RenderPass>& renderPass) = 0;
		virtual void EndRenderPass() = 0;
		virtual void WaitAndRender() = 0;

		virtual ~CommandBuffer() = default;
	};

	class CommandList
	{
	public:

		virtual Ref<CommandBuffer> CreateCommandBuffer(uint32_t count = 1) = 0;
		virtual void WaitAndRender() = 0;
		virtual ~CommandList() = default;
		
	};

}
