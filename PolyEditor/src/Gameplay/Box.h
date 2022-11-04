#pragma once
#include <spdlog/spdlog.h>

#include "Engine/Engine/ECS/GameObject.h"
#include "Engine/Engine/Gameplay/World.h"

namespace Polyboid
{
	class Triangle : public GameObject
	{
	public:

		void OnGameObjectConstruction() override
		{
			GameObject::OnGameObjectConstruction();


			AddOrReplaceComponent<Shape>(glm::vec4{ .76f, .23f, 0.45f, 1.0f });

			auto size = GetWorld()->GetGameObjects().size();

			spdlog::info("{0} objects number {1}", m_Name, size);

			auto& transform = GetComponent<Transform>();
			transform.Position.y += 2.0f;

		}
		void OnBeginPlay() override
		{
			GameObject::OnBeginPlay();
		}


		void OnUpdate(float dt) override
		{
			GameObject::OnUpdate(dt);

			GetComponent<Transform>().Position.y += dt * 2.0f;

		}

		void OnEndPlay() override
		{
			GameObject::OnEndPlay();
		}

	};

	class Square : public GameObject
	{

	private:
		std::vector<GameObject*> m_GameObjects;

	public:
		Square()
		{
			//AddComponent<Square>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });
		}

		void OnGameObjectConstruction() override;
		void OnBeginPlay() override;
		void OnUpdate(float dt) override;
		void OnEndPlay() override;
	};
	
}
