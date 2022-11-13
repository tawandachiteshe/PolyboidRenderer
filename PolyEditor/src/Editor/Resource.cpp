#include "Resource.h"

#include <filesystem>

namespace Polyboid
{
	Ref<ResourceData> Resource::m_sData = std::make_shared<ResourceData>();

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


		m_sData->m_Icons["play"] = Texture::MakeTexture2D(MakeIconsPath("play.png"));
		m_sData->m_Icons["stop"] = Texture::MakeTexture2D(MakeIconsPath("stop.png"));
		m_sData->m_Icons["folder"] = Texture::MakeTexture2D(MakeIconsPath("folder.png"));
		m_sData->m_Icons["file"] = Texture::MakeTexture2D(MakeIconsPath("file.png"));
		m_sData->m_Icons["game_object"] = Texture::MakeTexture2D(MakeIconsPath("game_object.png"));
		m_sData->m_Icons["camera"] = Texture::MakeTexture2D(MakeIconsPath("camera.png"));

	}

}
