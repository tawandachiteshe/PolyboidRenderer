#include "boidpch.h"
#include "FileSystem.h"

namespace Polyboid
{
	std::string FileSystem::GenCacheFilename(const std::string& path, const std::string& ext)
	{
		std::filesystem::path filePath = path;

		std::string temp = filePath.stem().string();
		std::string temp2 = filePath.parent_path().string();
		temp2.append("/");

		auto cachedImageExist = temp2.append(temp.append(ext));
		return cachedImageExist;
	}
}
