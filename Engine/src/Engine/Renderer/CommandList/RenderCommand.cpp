#include "boidpch.h"
#include "RenderCommand.h"

#include "Commands/RenderCommands.h"
#include "Engine/Renderer/RenderAPI.h"
#include "Engine/Renderer/PipelineState.h"
#include "Engine/Engine/Engine.h"
#include "Engine/Renderer/CommandList.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/SyncObjects.h"
#include "Platform/Vulkan/VkRenderAPI.h"
#include "Platform/Vulkan/Utils/VulkanDevice.h"

namespace Polyboid
{

	Ref<RenderCommandData> RenderCommand::s_Data = nullptr;

	void RenderCommand::SetCommandLists(const std::vector<Ref<CommandList>>& cmdLists)
	{
		s_Data->m_CommandLists = cmdLists;
	}

	void RenderCommand::Init(RenderAPI* context)
	{
		static auto renderData = std::make_shared<RenderCommandData>();
		s_Data = renderData;
		s_Data->m_Context = context;
		s_Data->m_Commands.reserve(2000);
	}

	void RenderCommand::AddCommand(const Ref<Command>& renderCommand)
	{
		for (auto cmdList : s_Data->m_CommandLists)
		{
			auto cmdBuffer = cmdList->GetCommandBufferAt(Renderer::GetCurrentFrame());

			if (cmdBuffer == nullptr)
			{
				__debugbreak();
			}

			renderCommand->SetContext(cmdBuffer);
			s_Data->m_Commands.emplace_back(renderCommand);
		}
		
	}

	void RenderCommand::SubmitCommandBuffer(const std::vector<Ref<CommandList>>& cmdList)
	{
		s_Data->m_Context->SubmitCommandBuffer(cmdList);
	}

	void RenderCommand::BeginCommands(const std::vector<Ref<CommandList>>& cmdList)
	{
		for (auto& _cmdList : cmdList)
		{
			_cmdList->GetCommandBufferAt(Renderer::GetCurrentFrame())->Begin();
		}
	}

	void RenderCommand::EndCommands(const std::vector<Ref<CommandList>>& cmdList)
	{
		for (auto& _cmdList : cmdList)
		{
			_cmdList->GetCommandBufferAt(Renderer::GetCurrentFrame())->End();
		}
	}


	void RenderCommand::WaitAndRender(const Ref<Semaphore>& _imageAvailable, const Ref<Semaphore>& _renderFinished, const Ref<Fence>& inFlight)
	{


		for (const auto& renderCommand : s_Data->m_Commands)
		{
			renderCommand->Execute();
		}

		s_Data->m_Context->SubmitCommandBuffer(s_Data->m_CommandLists, _imageAvailable, _renderFinished, inFlight);


		s_Data->m_Commands.clear();

	}
}
