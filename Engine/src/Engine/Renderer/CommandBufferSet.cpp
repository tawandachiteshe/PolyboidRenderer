#include "boidpch.h"
#include "CommandBufferSet.h"

#include "RenderAPI.h"
#include "Engine/Engine/Application.h"


namespace Polyboid
{
	Ref<CommandBufferSet> CommandBufferSet::Create(const CommandListSettings& settings)
	{

		return RenderAPI::Get()->CreateCommandList(settings);

	}
}
