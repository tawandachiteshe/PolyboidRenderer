#include "boidpch.h"

#include "World.h"

#include "GameInstance.h"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/quaternion.hpp"
#include "Engine/Engine/Math/Math.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Engine/ECS/GameObject.h"

namespace Polyboid
{
	World::World(const std::string& name): m_Name(name)
	{

		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3{ 0.0f, 2.0f, 0.0f }) * glm::scale(glm::mat4(1.0f), glm::vec3{1.0f,2.0f, 1.0f}) * glm::toMat4(glm::quat(glm::vec3{ 0, glm::radians(45.0f), 0.0f }));

		glm::vec3 pos;
		glm::quat ori;
		
		glm::vec3 scale;


		Math::DecomposeMatrix(pos, scale, ori, model);

		auto euler = glm::eulerAngles(ori);

	}

	void World::OnBeginPlay() const
	{
		for (auto& gameObject : m_GameObjects)
		{
			gameObject->OnBeginPlay();
		}
	}

	void World::OnUpdate(float ts) const
	{
		for (auto& gameObject : m_GameObjects)
		{
			gameObject->OnUpdate(ts);
		}
	}

	void World::OnEndPlay() const
	{
		for (auto& gameObject : m_GameObjects)
		{
			gameObject->OnEndPlay();
		}
	}

	void World::Render()
	{

		auto view = m_Registry.view<Transform, Shape>();

		const auto& camera = GameInstance::GetCurrentCamera();

		Renderer2D::BeginDraw(camera);

		for (auto entity : view)
		{

			auto [transform, shape] = view.get<Transform, Shape>(entity);

			Renderer2D::DrawQuad(transform.GetTransform(), shape.color);


		}

		Renderer2D::EndDraw();

	}
}
