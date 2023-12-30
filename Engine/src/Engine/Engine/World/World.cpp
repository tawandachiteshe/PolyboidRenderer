#include "boidpch.h"
#include "World.h"

#include "Components.h"
#include "GameObject.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Renderer/Renderer3D.h"


namespace Polyboid
{
	World::World(std::string name): m_Name(std::move(name))
	{
	}

	Ref<World> World::Create(const std::string& name)
	{
		return CreateRef<World>(name);
	}

	void World::Update(float ts)
	{
		//Render 2d geo here
	}

	GameObject World::CreateGameObject(const std::string& name)
	{
		const auto handle = m_Registry.create();

		GameObject gameObject(handle, this);
		gameObject.AddComponent<TagComponent>(name);
		gameObject.AddComponent<UUIDComponent>();
		gameObject.AddComponent<TransformComponent>();

		return gameObject;
	}

	void World::OnRender(const Ref<Camera>& camera)
	{
		{
			Renderer2D::BeginDraw(camera);


			for (auto& entity : m_Registry.view<TransformComponent, ShapeComponent>())
			{
				const auto& [transform, shape] = m_Registry.get<TransformComponent, ShapeComponent>(entity);
				Renderer2D::DrawQuad(transform.GetTransform(), shape.Color);
			}

			Renderer2D::EndDraw();
		}
	}
}
