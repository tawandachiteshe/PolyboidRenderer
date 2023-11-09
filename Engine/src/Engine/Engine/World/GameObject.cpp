#include "boidpch.h"
#include "GameObject.h"

#include "Components.h"


namespace Polyboid
{
	GameObject::GameObject(entt::entity entityId, World* world): m_World(world), m_Entity(entityId)
	{
	}

	bool GameObject::IsValid()
	{
		return m_Entity != entt::null && m_World != nullptr;
	}

	entt::entity GameObject::GetEntityHandle() const
	{
		return m_Entity;
	}

	std::string GameObject::GetName()
	{

		return GetComponent<TagComponent>().name;
	}

	UUID GameObject::GetUUID()
	{
		return GetComponent<UUIDComponent>().id;
	}
}
