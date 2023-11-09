#include "boidpch.h"
#include "World.h"

#include "Components.h"
#include "GameObject.h"


namespace Polyboid
{
	World::World(std::string name): m_Name(std::move(name))
	{
	}

	Ref<World> World::Create(const std::string& name)
	{
		return CreateRef<World>(name);
	}

	GameObject World::CreateGameObject(const std::string& name)
	{
		const auto handle = m_Registry.create();

		GameObject gameObject( handle, this );
		gameObject.AddComponent<TagComponent>(name);
		gameObject.AddComponent<UUIDComponent>();
		gameObject.AddComponent<TransformComponent>();

		return gameObject;

	}

}
