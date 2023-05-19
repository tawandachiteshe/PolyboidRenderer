#include "boidpch.h"
#include "RendererSyncObjects.h"

#include "SyncObjects.h"


namespace Polyboid
{
	RendererSyncObjects::RendererSyncObjects(uint32_t count)
	{
		for (uint32_t i = 0; i < count; ++i)
		{
			m_InFlightFences.push_back(Fence::Create());
			m_ImagesSemaphores.push_back(Semaphore::Create());
			m_RenderSemaphores.push_back(Semaphore::Create());
		}
	}

	Ref<Fence> RendererSyncObjects::GetInFlightFence(uint32_t index)
	{
		return m_InFlightFences.at(index);
	}

	Ref<Semaphore> RendererSyncObjects::GetImageSemaphore(uint32_t index)
	{
		return  m_ImagesSemaphores.at(index);
	}

	Ref<Semaphore> RendererSyncObjects::GetRenderSemaphore(uint32_t index)
	{
		return  m_RenderSemaphores.at(index);
	}

	Ref<RendererSyncObjects> RendererSyncObjects::Create(uint32_t count)
	{
		return std::make_shared<RendererSyncObjects>(count);
	}
}
