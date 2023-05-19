#pragma once
#include "Engine/Engine/Base.h"


namespace vk
{
	class CommandBuffer;
}

namespace Polyboid
{
	class Fence;
	class Semaphore;
	class CommandList;

#define   ALLOC_COMMAND(Klass, ...) std::make_shared<Klass>(__VA_ARGS__)

	class RenderPass;
	class PipelineState;
	class Command;
	class RenderAPI;
	class CommandBuffer;


	struct RenderCommandData
	{
		RenderAPI* m_Context{};
		std::vector<Ref<Command>> m_Commands;
		std::vector<Ref<CommandList>> m_CommandLists;
	};


	class RenderCommand
	{
	private:
		static Ref<RenderCommandData> s_Data;
	public:
		static void SetCommandLists(const std::vector<Ref<CommandList>>& cmdList);
		static void Init(RenderAPI* context);
		static void AddCommand(const Ref<Command>& renderCommand);
		static void SubmitCommandBuffer(const std::vector<Ref<CommandList>>& cmdList);
		static void BeginCommands(const std::vector<Ref<CommandList>>& cmdList);
		static void EndCommands(const std::vector<Ref<CommandList>>& cmdList);
		static void WaitAndRender(const Ref<Semaphore>& imageAvailable, const Ref<Semaphore>& renderFinished, const Ref<Fence>& inFlight);

	};

}
