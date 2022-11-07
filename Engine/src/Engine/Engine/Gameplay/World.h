#pragma once
#include <vector>
#include <string>

#include "Engine/Engine/ECS/GameObjectClass.h"
#include <entt/entt.hpp>


namespace Polyboid
{
	class GameObject;

	class World
	{


	public:
		World(const std::string& name = "World");
		void OnBeginPlay() const;
		void OnUpdate(float ts) const;
		void OnEndPlay() const;
		
		template<typename Class>
		GameObjectClass<Class>* CreateGameObject(const std::string& name = "GameObject");

		
		std::vector<GameObject*>& GetGameObjects() { return m_GameObjects; }

		std::string& GetName() { return m_Name; }

		void Render();

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

	template<typename Class>
	GameObjectClass<Class>* World::CreateGameObject(const std::string& name )
	{
		auto* gameObjectClass = new GameObjectClass<Class>();
		m_GameObjects.emplace_back(gameObjectClass);

		gameObjectClass->SetWorld(this);
		gameObjectClass->OnGameObjectConstruction();

		gameObjectClass->GetComponent<Tag>().name = name;

	

		
		return gameObjectClass;
	}
}
