#pragma once
#include <any>
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
		virtual void SubmitAndRender() = 0;
		virtual void SubmitSwapchain(const Ref<Swapchain>& swapchain) = 0;
		virtual void SubmitAndPresent() = 0;
		virtual std::any GetHandle() = 0;


		virtual ~CommandBuffer() = default;

		static Ref<CommandBuffer> Create();

	};


	class CommandList
	{
	public:

		virtual void CreateCommandBuffers(uint32_t count = 1) = 0;
		virtual Ref<CommandBuffer> GetCommandBufferAt(uint32_t index = 0) = 0;
		virtual bool CanPresent() = 0;
		virtual ~CommandList() = default;

		static Ref<CommandList> Create(bool canPresent = false);

	};

}
