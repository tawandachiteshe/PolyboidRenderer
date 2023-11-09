#pragma once
#include <any>

#include "Engine/Engine/Base.h"

namespace Polyboid
{

	class Semaphore
	{
	public:
		virtual  std::any GetHandle() = 0;
		virtual ~Semaphore() {}

		static Ref<Semaphore> Create();

	};

	class Fence
	{
	public:
		virtual  std::any GetHandle() = 0;
		virtual ~Fence() {}

		static Ref<Fence> Create();
	};


}

