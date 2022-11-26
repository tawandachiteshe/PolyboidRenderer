#include "WorldSerializer.h"

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
#include <fstream>

#include "Engine/Engine/Engine.h"
#include "Engine/Engine/ECS/GameObject.h"
#include "Engine/Engine/Gameplay/GameStatics.h"
#include "Engine/Engine/Gameplay/World.h"


namespace nlohmann
{
	template <>
	struct adl_serializer<glm::vec2>
	{
		static glm::vec2 from_json(const json& j)
		{
			auto vec = j.get<std::array<float, 2>>();
			glm::vec2 value{vec[0], vec[1]};

			return value;
		}


		static void to_json(json& j, glm::vec2 t)
		{
			std::array<float, 2> glmValues = {0};

			glmValues[0] = t.x;
			glmValues[1] = t.y;

			j = glmValues;
		}
	};


	template <>
	struct adl_serializer<glm::vec3>
	{
		static glm::vec3 from_json(const json& j)
		{
			auto vec = j.get<std::array<float, 3>>();
			glm::vec3 value{vec[0], vec[1], vec[2]};

			return value;
		}


		static void to_json(json& j, glm::vec3 t)
		{
			std::array<float, 3> glmValues = {0};

			glmValues[0] = t.x;
			glmValues[1] = t.y;
			glmValues[2] = t.z;

			j = glmValues;
		}
	};

	template <>
	struct adl_serializer<glm::vec4>
	{
		static glm::vec4 from_json(const json& j)
		{
			auto vec = j.get<std::array<float, 4>>();
			glm::vec4 value{vec[0], vec[1], vec[2], vec[3]};

			return value;
		}

		static void to_json(json& j, glm::vec4 t)
		{
			std::array<float, 4> glmValues = {0};

			glmValues[0] = t.x;
			glmValues[1] = t.y;
			glmValues[2] = t.z;
			glmValues[3] = t.w;

			j = glmValues;
		}
	};
}

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

		auto& world = GameStatics::GetCurrentWorld();
		std::ofstream outStream(directory);

		auto gameObjects = nlohmann::json::array({});

		if (outStream.is_open())
		{
			json["_name"] = world->GetName();


			for (auto gameObject : world->GetGameObjects())
			{
				auto gameObjectJson = nlohmann::json::object();
				auto gameObjectComponents = nlohmann::json::array();


				auto name = gameObject->GetComponent<TagComponent>();
				auto id = gameObject->GetComponent<IDComponent>();

				gameObjectJson["id"] = (uint64_t)id.id;
				gameObjectJson["name"] = name.name;

				// insert components here

				if (gameObject->HasComponent<ShapeComponent>())
				{
					auto shapeComponent = gameObject->GetComponent<ShapeComponent>();
					auto componentObject = nlohmann::json::object();
					auto componentData = nlohmann::json::object();


					auto& color = shapeComponent.color;
					componentData["color"] = color;

					componentObject["ShapeComponent"] = componentData;

					gameObjectComponents.push_back(componentObject);
				}

				if (gameObject->HasComponent<TransformComponent>())
				{
					auto transformComponent = gameObject->GetComponent<TransformComponent>();

					auto componentObject = nlohmann::json::object();
					auto componentData = nlohmann::json::object();


					componentData["position"] = transformComponent.Position;
					componentData["scale"] = transformComponent.Scale;
					componentData["rotation"] = transformComponent.Rotation;

					componentObject["TransformComponent"] = componentData;


					gameObjectComponents.push_back(componentObject);
				}


				gameObjectJson["components"] = gameObjectComponents;

				gameObjects.push_back(gameObjectJson);
			}

			json["game_objects"] = gameObjects;

			outStream << std::setw(2) << json;
			outStream.close();
		}
	}

	void WorldSerializer::Deserialize(const std::filesystem::path& path)
	{
		std::ifstream inStream(path);
		auto levelName = path.stem().string();
		auto& json = s_Data.m_Json;


		auto world = Engine::CreateWorld(levelName);
		GameStatics::SetCurrentWorld(world);

		if (inStream.is_open())
		{
			inStream >> json;

			for (auto& gameObject : json["game_objects"])
			{
				auto name = gameObject["name"].get<std::string>();
				auto id = gameObject["id"].get<uint64_t>();
				auto gameObjectEntity = world->CreateGameObject(name, id);

				for (auto& components : gameObject["components"])
				{
					//TransformComponent
					if (components.contains("TransformComponent")) 
					{
						auto transformComponentJson = components["TransformComponent"];
						auto position = transformComponentJson["position"].get<glm::vec3>();
						auto scale = transformComponentJson["scale"].get<glm::vec3>();
						auto rotation = transformComponentJson["rotation"].get<glm::vec3>();

						auto& transform = gameObjectEntity->GetComponent<TransformComponent>();
						transform.Position = position;
						transform.Scale = scale;
						transform.Rotation = rotation;
					}

					if (components.contains("ShapeComponent"))
					{
						auto shapeComponent = components["ShapeComponent"];
						auto color = shapeComponent["color"].get<glm::vec4>();

						gameObjectEntity->AddComponent<ShapeComponent>(color);
					}

				}
			}


			inStream.close();
		}
	}
}
