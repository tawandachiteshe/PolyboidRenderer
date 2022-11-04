#include "Box.h"

#include <spdlog/spdlog.h>

#include "Engine/Engine/ECS/Components.h"
#include "Engine/Engine/Gameplay/World.h"

namespace Polyboid
{
	void Square::OnGameObjectConstruction()
	{
		GameObject::OnGameObjectConstruction();

		AddOrReplaceComponent<Shape>(glm::vec4{ 1.0f, .23f, 0.45f, 1.0f });

		auto size = GetWorld()->GetGameObjects().size();

		spdlog::info("{0} objects number {1}", m_Name, size);

		auto& transform = GetComponent<Transform>();
		transform.Position.y += 0.2f;

	}

	void Square::OnBeginPlay()
	{
		GameObject::OnBeginPlay();

	}

	void Square::OnUpdate(float dt)
	{
		GameObject::OnUpdate(dt);

		
		auto& transform = GetComponent<Transform>();
		transform.Rotation.z += dt * 10.0f;

		
	}

	void Square::OnEndPlay()
	{
		GameObject::OnEndPlay();
		RemoveComponent<Square>();

		GetComponent<Transform>().Rotation = { 0.0f, 0.0f, 0.0f };

		spdlog::info("On box end");
	}
}
