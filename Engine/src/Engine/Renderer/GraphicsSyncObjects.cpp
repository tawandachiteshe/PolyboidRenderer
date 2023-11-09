#include "boidpch.h"
#include "GraphicsSyncObjects.h"

#include "RenderCommand.h"
#include "SyncObjects.h"


namespace Polyboid
{
	GraphicsSyncObjects::GraphicsSyncObjects(uint32_t count)
	{
		for (uint32_t i = 0; i < count; ++i)
		{
			m_InFlightFences.push_back(Fence::Create());
			m_ImagesSemaphores.push_back(Semaphore::Create());
			m_RenderSemaphores.push_back(Semaphore::Create());
		}
	}

	Ref<Fence> GraphicsSyncObjects::GetInFlightFence(uint32_t index)
	{
		return m_InFlightFences.at(index);
	}

	Ref<Semaphore> GraphicsSyncObjects::GetImageSemaphore(uint32_t index)
	{
		return  m_ImagesSemaphores.at(index);
	}

	Ref<Semaphore> GraphicsSyncObjects::GetRenderSemaphore(uint32_t index)
	{
		return  m_RenderSemaphores.at(index);
	}

	Ref<GraphicsSyncObjects> GraphicsSyncObjects::Create()
	{
		return CreateRef<GraphicsSyncObjects>(RenderCommand::GetMaxFramesInFlight());
	}


	//Compute start here

	ComputeSyncObjects::ComputeSyncObjects(uint32_t count)
	{
		for (uint32_t i = 0; i < count; ++i)
		{
			m_InFlightFences.push_back(Fence::Create());
			m_ComputeFinishedSemaphores.push_back(Semaphore::Create());
		}
	}

	Ref<Fence> ComputeSyncObjects::GetInFlightFence(uint32_t index)
	{
		return m_InFlightFences.at(index);
	}

	Ref<Semaphore> ComputeSyncObjects::GetComputeFinishedSemaphore(uint32_t index)
	{
		return m_ComputeFinishedSemaphores.at(index);
	}

	Ref<ComputeSyncObjects> ComputeSyncObjects::Create()
	{
		return CreateRef<ComputeSyncObjects>(RenderCommand::GetMaxFramesInFlight());
	}
}
