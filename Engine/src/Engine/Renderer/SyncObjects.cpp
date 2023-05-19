#include "boidpch.h"
#include "SyncObjects.h"

#include "RenderAPI.h"
#include "Engine/Engine/Application.h"

namespace Polyboid
{
	Ref<Semaphore> Semaphore::Create()
	{
		auto renderAPi = RenderAPI::Get();
		return  renderAPi->CreateGraphicsSemaphore();
	}

	Ref<Fence> Fence::Create()
	{
		auto renderAPi = RenderAPI::Get();
		return renderAPi->CreateGraphicsFence();
	}
}
