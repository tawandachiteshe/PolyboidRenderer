#pragma once
#include <any>

namespace Polyboid
{

	class Semaphore
	{
	public:
		virtual  std::any GetHandle() = 0;
		virtual ~Semaphore() {}

	};

	class Fence
	{
	public:
		virtual  std::any GetHandle() = 0;
		virtual ~Fence() {}
	};


}

