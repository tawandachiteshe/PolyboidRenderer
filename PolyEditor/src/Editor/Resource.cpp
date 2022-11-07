#include "Resource.h"

#include <filesystem>

namespace Polyboid
{
	Ref<ResourceData> Resource::m_sData = std::make_shared<ResourceData>();

	static std::filesystem::path s_ResourcePath = "Resources";

	static  std::filesystem::path s_IconsPath = "/Icons/";

	void Resource::Init()
	{

		auto iconsDir = s_ResourcePath += s_IconsPath;

		m_sData->m_Icons["play"] = Texture::MakeTexture2D(iconsDir.string() + "play.png");
		m_sData->m_Icons["stop"] = Texture::MakeTexture2D(iconsDir.string() + "stop.png");
		m_sData->m_Icons["folder"] = Texture::MakeTexture2D(iconsDir.string() + "folder.png");
		m_sData->m_Icons["file"] = Texture::MakeTexture2D(iconsDir.string() + "file.png");

	}

}
