#include "boidpch.h"

#include "Engine/Engine/ECS/GameObject.h"
#include "World.h"

#include <spdlog/spdlog.h>

#include "GameStatics.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Engine/ECS/Components.h"


namespace Polyboid
{
	World::World(const std::string& name): m_Name(name)
	{
		
		m_GameObjects.reserve(200000);
	}

	void World::OnBeginPlay() const
	{
		for (const auto gameObject : m_GameObjects)
		{
			gameObject->OnBeginPlay();
		}

	}

	void World::OnUpdate(float ts) const
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
		
		for (;objects != std::end(m_GameObjects); ++objects)
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
			spdlog::info("Game object {}", tempObject->GetComponent<TagComponent>().name);
			return tempObject;
		}
		else
		{
			spdlog::info("Not found");
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
			spdlog::info("Game object {}", tempObject->GetComponent<TagComponent>().name);
			return tempObject;
		}
		else
		{
			spdlog::info("Not found");
		}

		return nullptr;
	}

	std::vector<GameObject*> World::FindGameObjectsByName(const std::string& name)
	{
		
		std::vector<GameObject*> objects;


		std::for_each(begin(), end(), [&](GameObject* value)
			{
				auto& tag = value->GetComponent<TagComponent>();
				if(tag.name == name)
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

	void World::Render()
	{

		
		auto cameraView = m_Registry.view<TransformComponent, CameraComponent>();

		for (const auto entity : cameraView)
		{
			auto [transform, camera] = cameraView.get<TransformComponent, CameraComponent>(entity);

			if (camera.Camera)
			{
				GameStatics::SetCurrentCamera(camera.Camera);
				//spdlog::info("camera With id {}", static_cast<int32_t>(entity));
			}

		}

		const auto& camera = GameStatics::GetCurrentCamera();

		Renderer2D::BeginDraw(camera);

		auto view = m_Registry.view<TransformComponent, ShapeComponent>();

		for (auto entity : view)
		{

			auto [transform, shape] = view.get<TransformComponent, ShapeComponent>(entity);


			switch (shape.type)
			{
			case ShapeType::Quad:
				Renderer2D::DrawQuad(transform.GetTransform(), shape.color);
				break;
			case ShapeType::Circle:
				Renderer2D::DrawCircle(transform.GetTransform(), shape.color, shape.thickness, shape.fade);
			default:
				break;
			}

			


		}

		Renderer2D::EndDraw();

	}
}
