#include "WorldSerializer.h"

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
#include <fstream>

#include "Engine/Engine/ECS/GameObject.h"
#include "Engine/Engine/Gameplay/GameInstance.h"
#include "Engine/Engine/Gameplay/World.h"


namespace Polyboid
{
	struct SerializerData
	{
		nlohmann::json m_Json;
	};

	static std::string fileExtension = ".pmap";
	static SerializerData s_Data;

	void WorldSerializer::Serialize(const std::filesystem::path& directory)
	{

		auto& json = s_Data.m_Json;

		
		auto& world = GameInstance::GetCurrentWorld();
		auto name = world->GetName();
		std::ofstream outStream(directory/ name.append(fileExtension));

		auto gameObjects = nlohmann::json::array({});
		auto gameObject = nlohmann::json::object();
		gameObject["id"] = 25;
		gameObject["Position"] = { 1, 2, 3 };

		gameObjects.push_back(gameObject);

		if (outStream.is_open())
		{
			json["_name"] = world->GetName();
			json["game_objects"] = gameObjects;

			spdlog::info("{}", json.dump());
			
			outStream << std::setw(2) << json;




			outStream.close();
		}


		

	}

	void WorldSerializer::Deserialize(const std::filesystem::path& path)
	{
		std::ifstream inStream(path);
	}
}
