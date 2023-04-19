#include "boidpch.h"
#include "RenderCommand.h"

#include "Commands/RenderCommands.h"
#include "Engine/Renderer/RenderAPI.h"
#include "Engine/Renderer/PipelineState.h"
#include "Engine/Engine/Engine.h"
#include "Engine/Renderer/CommandList.h"
#include "Engine/Renderer/Renderer.h"

namespace Polyboid
{

	Unique<RenderCommandData> RenderCommand::s_Data = nullptr;

	void RenderCommand::SetCommandLists(const std::vector<Ref<CommandList>>& cmdLists)
	{
		s_Data->m_CommandLists = cmdLists;
	}

	void RenderCommand::Init(RenderAPI* context)
	{
		s_Data = std::make_unique<RenderCommandData>();
		s_Data->m_Context = context;
		s_Data->m_Commands.reserve(2000);
	}

	void RenderCommand::AddCommand(const Ref<Command>& renderCommand)
	{
		for (auto cmdList : s_Data->m_CommandLists)
		{
			auto cmdBuffer = cmdList->GetCommandBufferAt(Renderer::GetCurrentFrame());
			renderCommand->SetContext(cmdBuffer);
			s_Data->m_Commands.emplace_back(renderCommand);
		}
		
	}

	void RenderCommand::WaitAndRender()
	{
		for (const auto& renderCommand : s_Data->m_Commands)
		{
			renderCommand->Execute();
		}

		for (auto cmdList : s_Data->m_CommandLists)
		{
			auto cmdBuffer = cmdList->GetCommandBufferAt(Renderer::GetCurrentFrame());
			
			cmdBuffer->SubmitAndRender();

			if (cmdList->CanPresent())
			{
				cmdBuffer->SubmitAndPresent();
			}

			
		}


		s_Data->m_Commands.clear();

	}
}
