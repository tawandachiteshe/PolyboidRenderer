#pragma once
#include "Engine/Engine/Base.h"
#include <vector>

namespace Polyboid
{
	class Fence;
	class Semaphore;

	class GraphicsSyncObjects
	{
	private:
		std::vector<Ref<Fence>> m_InFlightFences;
		std::vector<Ref<Semaphore>> m_ImagesSemaphores;
		std::vector<Ref<Semaphore>> m_RenderSemaphores;
	public:
		explicit GraphicsSyncObjects(uint32_t count);

		Ref<Fence> GetInFlightFence(uint32_t index);
		Ref<Semaphore> GetImageSemaphore(uint32_t index);
		Ref<Semaphore> GetRenderSemaphore(uint32_t index);

		static Ref<GraphicsSyncObjects> Create();

	};

	class ComputeSyncObjects
	{
	private:
		std::vector<Ref<Fence>> m_InFlightFences;
		std::vector<Ref<Semaphore>> m_ComputeFinishedSemaphores;
	public:
		explicit ComputeSyncObjects(uint32_t count);

		Ref<Fence> GetInFlightFence(uint32_t index);
		Ref<Semaphore> GetComputeFinishedSemaphore(uint32_t index);

		static Ref<ComputeSyncObjects> Create();

	};


}

