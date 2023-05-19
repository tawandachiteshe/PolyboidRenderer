#pragma once
#include "Engine/Engine/Base.h"


namespace Polyboid
{
	class Fence;
	class Semaphore;

	class RendererSyncObjects
	{
	private:
		std::vector<Ref<Fence>> m_InFlightFences;
		std::vector<Ref<Semaphore>> m_ImagesSemaphores;
		std::vector<Ref<Semaphore>> m_RenderSemaphores;
	public:
		explicit RendererSyncObjects(uint32_t count);

		Ref<Fence> GetInFlightFence(uint32_t index);
		Ref<Semaphore> GetImageSemaphore(uint32_t index);
		Ref<Semaphore> GetRenderSemaphore(uint32_t index);

		static Ref<RendererSyncObjects> Create(uint32_t count);

	};

}

