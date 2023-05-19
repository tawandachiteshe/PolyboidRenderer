#include "boidpch.h"
#include "CommandList.h"

#include "RenderAPI.h"
#include "Engine/Engine/Application.h"


namespace Polyboid
{
	Ref<CommandList> CommandList::Create(const CommandListSettings& settings)
	{

		return RenderAPI::Get()->CreateCommandList(settings);

	}
}
