#include "boidpch.h"
#include "CommandList.h"

#include "RenderAPI.h"
#include "Engine/Engine/Application.h"


namespace Polyboid
{
	Ref<CommandList> CommandList::Create(bool canPresent)
	{

		return Application::Get().GetRenderAPI()->CreateCommandList();

	}
}
