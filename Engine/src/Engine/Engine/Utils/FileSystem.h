#pragma once


namespace Polyboid
{
	class FileSystem
	{
	public:
		static std::string GenCacheFilename(const std::string& path, const std::string& ext = ".cached");
	};
}
