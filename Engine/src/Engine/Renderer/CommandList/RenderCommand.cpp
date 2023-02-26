#include "boidpch.h"
#include "RenderCommand.h"

#include "Commands/RenderCommands.h"
#include "Engine/Renderer/RenderAPI.h"
#include "Engine/Renderer/PipelineState.h"
#include "Engine/Engine/Engine.h"

namespace Polyboid
{

	Unique<RenderCommandData> RenderCommand::s_Data = nullptr;

	void RenderCommand::Init(const Ref<RenderAPI>& context)
	{
		s_Data = std::make_unique<RenderCommandData>();
		s_Data->m_Context = context;
		s_Data->m_Commands.reserve(2000);
	}

	void RenderCommand::AddCommand(const Ref<Command>& renderCommand)
	{
		renderCommand->SetContext(s_Data->m_Context);
		s_Data->m_Commands.emplace_back(renderCommand);
	}

	void RenderCommand::WaitAndRender()
	{
		for (const auto& renderCommand : s_Data->m_Commands)
		{
			renderCommand->Execute();
		}

		s_Data->m_Commands.clear();


	}
}
