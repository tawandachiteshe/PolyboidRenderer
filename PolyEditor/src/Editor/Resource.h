#pragma once
#include <unordered_map>

#include "Engine/Engine/Base.h"
#include "Engine/Renderer/Texture2D.h"

namespace Polyboid
{

	using ResourceMap = std::unordered_map<std::string, Ref<Texture>>;

	struct ResourceData
	{
		ResourceMap m_Icons;
	};

	class Resource
	{

	private:
		static Ref<ResourceData> m_sData;

	public:
		static void Init();
		static ResourceMap& GetIcons()
		{
			return m_sData->m_Icons;
		}

	};

}
