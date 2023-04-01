#include "boidpch.h"
#include "RenderCommand.h"

#include "Commands/RenderCommands.h"
#include "Engine/Renderer/RenderAPI.h"
#include "Engine/Renderer/PipelineState.h"
#include "Engine/Engine/Engine.h"
#include "Engine/Renderer/CommandList.h"

namespace Polyboid
{

	Unique<RenderCommandData> RenderCommand::s_Data = nullptr;

	void RenderCommand::Init(RenderAPI* context)
	{
		s_Data = std::make_unique<RenderCommandData>();
		s_Data->m_Context = context;
		s_Data->m_Commands.reserve(2000);

		auto commandList = context->CreateCommandList();
		s_Data->m_CommandBuffer = commandList->CreateCommandBuffer();
	}

	void RenderCommand::AddCommand(const Ref<Command>& renderCommand)
	{
		renderCommand->SetContext(s_Data->m_CommandBuffer);
		s_Data->m_Commands.emplace_back(renderCommand);
	}

	void RenderCommand::WaitAndRender()
	{
		for (const auto& renderCommand : s_Data->m_Commands)
		{
			renderCommand->Execute();
		}

		s_Data->m_CommandBuffer->WaitAndRender();

		s_Data->m_Commands.clear();

	}
}
