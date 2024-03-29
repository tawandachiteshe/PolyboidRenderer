#pragma once
#include <vector>
#include <string>

#include <entt/entt.hpp>

#include "GameStatics.h"
#include "Engine/Engine/ECS/GameObject.h"

namespace Polyboid
{
	class UUID;
	class WorldRenderer;
	class Shader;
	class GameObject;

	template<typename T, typename ... Args>
	class TGameObject : public T
	{
	public:
		TGameObject(Args&&... args) : T(std::forward<Args>(args) ...)
		{

		}

	};

	class World
	{
	private:
		double m_WorldRenderTime = 0.0;

	public:
		double GetRenderTime() { return m_WorldRenderTime; }
		World(const std::string& name = "World");

		void InitRenderer();

		void UpdateLights();

		void OnBeginPlay() const;
		void OnUpdate(float ts);
		void OnEndPlay() const;
		~World();

		std::vector<GameObject*>::iterator begin() { return m_GameObjects.begin(); }
		std::vector<GameObject*>::iterator end() { return m_GameObjects.end(); }

		void DestroyGameObject(GameObject* gameObject);
		void DestroyGameObjectByName(const std::string& name);
		void DestroyGameObjectsByName(const std::string& name);
		void DestroyGameObjectWithId(UUID id);
		void DestroyGameObjects(std::vector<GameObject*>::iterator& objects);
		void DestroyGameObject(std::vector<GameObject*>::iterator& object);

		GameObject* FindGameObjectByName(const std::string& name);
		GameObject* FindGameObjectByID(const UUID& id);
		std::vector<GameObject*> FindGameObjectsByName(const std::string& name);

		template<typename Class>
		TGameObject<Class>* CreateGameObject(const std::string& name = "GameObject", uint64_t uuid = 0)
		{
			
			auto id = CreateEntityID();
			auto* gameObjectClass = new TGameObject<Class>();
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

		GameObject* CreateGameObject(const std::string& name = "GameObject", uint64_t id = 0);
		
		std::vector<GameObject*>& GetGameObjects() { return m_GameObjects; }

		std::string& GetName() { return m_Name; }

		void Render(float dt);

		void Update(float dt);

		entt::registry& GetRegistry() { return m_Registry; }

		uint32_t CreateEntityID() { return static_cast<uint32_t>(m_Registry.create()); }

		template <class  Component>
		Component& GetComponent(uint32_t gameObjectId)
		{
			auto& registry = GetRegistry();

			auto view = registry.view<Component>();

			return view.get<Component>(static_cast<const entt::entity>(gameObjectId));

		}


	private:

		entt::registry m_Registry;

		std::string m_Name;
		//find a better data structure for this:::
		std::vector<GameObject*> m_GameObjects;
		
	};


}
