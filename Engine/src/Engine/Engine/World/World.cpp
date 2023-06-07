#include "boidpch.h"

#include "World.h"


#include <spdlog/spdlog.h>
#include "GameStatics.h"
#include "Engine/Engine/RenderingData.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Engine/Math/Math.h"
#include "Engine/Engine/MeshImporter/MeshImporter.h"
#include "Engine/Renderer/RenderCommand.h"
#include "Engine/Renderer/WorldRenderer.h"
#include "Engine/Engine/ECS/Components.h"
#include "Engine/Engine/ECS/GameObject.h"
#include <tuple>

#include "Engine/Renderer/WorldRendererLegacy.h"


namespace Polyboid
{
	struct CalculateLightView
	{
		glm::vec3 m_Rotation;
		glm::vec3 m_Position;
		glm::vec3 m_FocalPoint{0.0f, 0.0f, 0.0f};
		glm::mat4 m_View = glm::mat4(1.0f);

		CalculateLightView(const TransformComponent& transform) : m_Rotation(transform.Rotation),
		                                                          m_Position(transform.Position)
		{
			glm::quat orientation = GetOrientation();
			m_View = glm::translate(glm::mat4(1.0f), m_FocalPoint - GetForwardDirection() * m_Position.z) * glm::toMat4(
				orientation);
			m_View = glm::inverse(m_View);
		}

		glm::mat4& GetView()
		{
			return m_View;
		}

		glm::vec3 CalculatePosition() const
		{
			//forcal point might be used if press F maybe
			return m_FocalPoint - GetForwardDirection() * m_Position;
		}

		glm::vec3 GetUpDirection() const
		{
			return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
		}

		glm::vec3 GetRightDirection() const
		{
			return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
		}

		glm::vec3 GetForwardDirection() const
		{
			return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
		}

		glm::quat GetOrientation() const
		{
			return {glm::vec3(m_Rotation.x, m_Rotation.y, 0.0f)};
		}
	};

	World::World(const std::string& name): m_Name(name)
	{
		m_GameObjects.reserve(200000);


	}

	void World::InitRenderer()
	{

	}

	void World::UpdateLights()
	{
	
		const auto pointLightView = m_Registry.view<TransformComponent, PointLightComponent>();
		const auto spotLightView = m_Registry.view<TransformComponent, SpotLightComponent>();
		const auto dirLightView = m_Registry.view<TransformComponent, DirectionLightComponent>();

		const auto& camera = GameStatics::GetCurrentCamera();


		LightCount lightCount = {};
		std::vector<PointLightData> pointData;
		std::vector<SpotLightData> spotData;
		std::vector<DirectionalLightData> dirData;

		for (auto& entity : pointLightView)
		{
			auto [transform, light] = pointLightView.get<TransformComponent, PointLightComponent>(entity);

			PointLightData data = {};
			data.Position = transform.Position;
			data.PositionVS = camera->GetView() * glm::vec4(transform.Position, 0.0);
			data.Color = light.color;
			data.Distance = light.Distance;
			data.Energy = light.Energy;
			data.Enabled = light.Enabled;

			lightCount.Point++;

			pointData.push_back(data);

		}

		for (auto& entity : spotLightView)
		{

			auto [transform, spotLight] = spotLightView.get<TransformComponent, SpotLightComponent>(entity);

		

			SpotLightData data{};


			//MAke sure w component is zero important lesson when transforming with view space

			data.Position = transform.Position;
			data.PositionVS = camera->GetView() * glm::vec4(transform.Position, 0.0);
			data.DirectionVS = glm::normalize(camera->GetView() * glm::vec4(transform.Rotation, 0.0));
			data.Direction = transform.Rotation;
			data.Color = spotLight.color;
			data.Distance = spotLight.Distance;
			data.Energy = spotLight.Energy;
			data.InnerAngle = spotLight.InnerAngle;
			data.OuterAngle = spotLight.OuterAngle;
			data.Enabled = spotLight.Enabled;

			lightCount.Spot++;

			spotData.push_back(data);
		}

		for (auto& entity : dirLightView)
		{

			auto [transform, light] = dirLightView.get<TransformComponent, DirectionLightComponent>(entity);

			DirectionalLightData data{};
			data.DirectionVS = camera->GetView() * glm::vec4(transform.Rotation, 0.0);
			data.DirectionVS = glm::normalize(data.DirectionVS);
			data.Direction = glm::normalize(data.DirectionVS);
			data.Color = light.color;
			data.Energy = light.Energy;
			data.Enabled = light.Enabled;

			lightCount.Dir++;

			dirData.push_back(data);
		}

		//
		// m_Renderer->SubmitLights({pointData, spotData, dirData});
		// m_Renderer->SubmitLightCount(lightCount);



	}

	void World::OnBeginPlay() const
	{
		for (const auto gameObject : m_GameObjects)
		{
			gameObject->OnBeginPlay();
		}
	}

	void World::OnUpdate(float ts)
	{
	
		for (const auto gameObject : m_GameObjects)
		{
			gameObject->OnUpdate(ts);
		}

	}

	void World::OnEndPlay() const
	{
		for (const auto gameObject : m_GameObjects)
		{
			gameObject->OnEndPlay();
		}
	}

	World::~World()
	{
	}

	void World::DestroyGameObject(GameObject* gameObject)
	{
		auto it = std::find(begin(), end(), gameObject);

		DestroyGameObject(it);
	}

	void World::DestroyGameObjectByName(const std::string& name)
	{
		auto it = std::find_if(begin(), end(), [&](GameObject* value)
		{
			auto& tag = value->GetComponent<TagComponent>();
			spdlog::info("entity with tag {}", tag.name);

			return tag.name == name;
		});

		DestroyGameObject(it);
	}


	void World::DestroyGameObjectsByName(const std::string& name)
	{
		while (FindGameObjectByName(name))
		{
			auto it = std::find_if(begin(), end(), [&](GameObject* gameObject)
			{
				auto& tag = gameObject->GetComponent<TagComponent>();
				spdlog::info("entity with tag {}", tag.name);
				return tag.name == name;
			});
			DestroyGameObject(it);
		}
	}

	void World::DestroyGameObjectWithId(UUID id)
	{
		auto it = std::find_if(begin(), end(), [&](GameObject* value)
		{
			auto& id = value->GetComponent<IDComponent>();
			return static_cast<uint64_t>(id.id) == static_cast<uint64_t>(id.id);
		});


		DestroyGameObject(it);
	}

	void World::DestroyGameObjects(std::vector<GameObject*>::iterator& objects)
	{
		for (; objects != std::end(m_GameObjects); ++objects)
		{
			GameObject* temp = *objects;
			temp->OnDestroy();
		}

		m_GameObjects.erase(objects);
	}

	void World::DestroyGameObject(std::vector<GameObject*>::iterator& object)
	{
		if (object != m_GameObjects.end())
		{
			GameObject* temp = *object;
			temp->OnDestroy();

			m_GameObjects.erase(object);
		}
	}


	GameObject* World::FindGameObjectByName(const std::string& name)
	{
		auto it = std::find_if(begin(), end(), [&](GameObject* value)
		{
			auto& tag = value->GetComponent<TagComponent>();
			return tag.name == name;
		});

		if (it != std::end(m_GameObjects))
		{
			GameObject* tempObject = *it;
			return tempObject;
		}

		return nullptr;
	}

	GameObject* World::FindGameObjectByID(const UUID& id)
	{
		auto it = std::find_if(begin(), end(), [&](GameObject* value)
		{
			auto& idComponent = value->GetComponent<IDComponent>();
			return id == idComponent.id;
		});

		if (it != std::end(m_GameObjects))
		{
			GameObject* tempObject = *it;
			return tempObject;
		}

		return nullptr;
	}

	std::vector<GameObject*> World::FindGameObjectsByName(const std::string& name)
	{
		std::vector<GameObject*> objects;


		std::for_each(begin(), end(), [&](GameObject* value)
		{
			auto& tag = value->GetComponent<TagComponent>();
			if (tag.name == name)
			{
				objects.emplace_back(value);
			}
		});

		return objects;
	}

	GameObject* World::CreateGameObject(const std::string& name, uint64_t uuid)
	{
		auto id = CreateEntityID();

		auto* gameObjectClass = new GameObject(static_cast<entt::entity>(id));
		gameObjectClass->SetID(id);

		gameObjectClass->SetWorld(GameStatics::GetCurrentWorld());

		//default components here
		gameObjectClass->AddComponent<TagComponent>(name);
		gameObjectClass->AddComponent<TransformComponent>();

		if (uuid != 0)
		{
			gameObjectClass->AddComponent<IDComponent>(uuid);
		}
		else
		{
			gameObjectClass->AddComponent<IDComponent>();
		}


		gameObjectClass->OnCreate();


		m_GameObjects.emplace_back(gameObjectClass);

		return gameObjectClass;
	}

	void World::Render(float dt)
	{
		//m_Renderer->Render();
	}

	void World::Update(float dt)
	{
		UpdateLights();
	}
}
