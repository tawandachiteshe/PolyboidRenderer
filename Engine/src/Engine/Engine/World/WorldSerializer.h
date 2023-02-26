#pragma once
#include <filesystem>

namespace Polyboid
{

	class WorldSerializer
	{


	private:
		void SerializeGameObject();


	public:
		static void Serialize(const std::filesystem::path& worldName);
		static void Deserialize(const std::filesystem::path& path);


	};


}
