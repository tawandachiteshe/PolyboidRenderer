#pragma once
#include <unordered_map>

#include "Engine/Engine/Base.h"

namespace Polyboid
{

	struct ResourceData
	{
	};

	class Resource
	{

	private:
		static Ref<ResourceData> m_sData;

	public:
		static void Init();
		

	};

}
