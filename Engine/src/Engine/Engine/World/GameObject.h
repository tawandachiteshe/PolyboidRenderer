#pragma once

#include <entt/entt.hpp>

#include "World.h"
#include "Engine/Engine/UUID.h"


namespace Polyboid
{
	struct TransformComponent;

	class GameObject
	{
	public:


		template<typename T>
		T& GetComponent()
		{
			return m_World->m_Registry.get<T>(m_Entity);
		}



		template<typename T, typename ... Args>
		T& AddComponent(Args&& ... args)
		{
			return  m_World->m_Registry.emplace<T>(m_Entity, std::forward<Args>(args)...);
		}

		template<typename ... Component>
		bool HasComponent()
		{
			return m_World->m_Registry.all_of<Component...>(m_Entity);
		}

		GameObject() = default;
		GameObject(entt::entity entityId, World* world);
		GameObject(const GameObject& other) = default;

		TransformComponent& GetTransform();

		bool IsValid();

		entt::entity GetEntityHandle() const;
		std::string GetName();
		UUID GetUUID();

	private:
		World* m_World = nullptr;
		entt::entity m_Entity = entt::null;


	};



}

