#include "boidpch.h"
#include "SyncObjects.h"

#include "RenderAPI.h"
#include "Engine/Engine/Application.h"

namespace Polyboid
{
	Ref<Semaphore> Semaphore::Create()
	{
		auto renderAPi = Application::Get().GetRenderAPI();
		return  renderAPi->CreateGraphicsSemaphore();
	}

	Ref<Fence> Fence::Create()
	{
		auto renderAPi = Application::Get().GetRenderAPI();
		return renderAPi->CreateGraphicsFence();
	}
}
