#include "Box.h"

#include <spdlog/spdlog.h>

#include "Engine/Engine/ECS/Components.h"
#include "Engine/Engine/Gameplay/World.h"

namespace Polyboid
{
	void Square::OnCreate()
	{
		GameObject::OnCreate();
		GetWorld()->CreateGameObject("Tawnada");
		GetWorld()->CreateGameObject("Red");
		if (!HasComponent<ShapeComponent>())
		{
			AddComponent<ShapeComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });
		}

	}

	void Square::OnBeginPlay()
	{
		GameObject::OnBeginPlay();

	

		m_Red = GetWorld()->FindGameObjectByName("Red");

		if (m_Red)
		{
			m_Red->AddComponent<ShapeComponent>();

			m_Red->GetComponent<TransformComponent>();

		}
		else
		{
			m_Red = GetWorld()->CreateGameObject<Square>("Red");
		}

		

	}

	void Square::OnUpdate(float dt)
	{
		GameObject::OnUpdate(dt);

		
	}

	void Square::OnEndPlay()
	{
		GameObject::OnEndPlay();

		spdlog::info("On box end");
		GetWorld()->DestroyGameObject(m_Red);
		m_Red = nullptr;
	}
}
