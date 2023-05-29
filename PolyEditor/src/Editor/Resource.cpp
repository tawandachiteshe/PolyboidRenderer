#include "Resource.h"

#include <filesystem>

namespace Polyboid
{
	Ref<ResourceData> Resource::m_sData = CreateRef<ResourceData>();

	static std::filesystem::path s_ResourcePath = "Resources";

	static  std::filesystem::path s_IconsPath = "Icons";

	static std::string MakeIconsPath(const std::string& iconName)
	{
		auto iconsDir = s_ResourcePath / s_IconsPath;
		auto iconFile = iconsDir / iconName;

		return iconFile.string();
	}

	void Resource::Init()
	{


	}

}
