#pragma once
#include <mono/metadata/object.h>


#define ADD_MONO_INTERNAL_CALL(x) mono_add_internal_call(std::string("Polyboid.Internals::").append(std::string(#x)).c_str(), x)
#include "Engine/Engine/Input.h"
#include "Engine/Engine/ECS/GameObject.h"
#include "glm/vec3.hpp"
#include "Engine/Engine/World/World.h"

namespace Polyboid
{

	static void Transform_SetPosition(uint64_t* enttId, glm::vec3* position)
	{
		// auto gameObject = GameStatics::GetCurrentWorld()->FindGameObjectByID(*enttId);
		//
		// auto& transform = gameObject->GetComponent<TransformComponent>();
		// transform.Position = *position;
	}

	static void Transform_GetPosition(uint64_t* enttId, glm::vec3* position)
	{
		spdlog::info("Transform Get... {}", *enttId);

		// auto gameObject = GameStatics::GetCurrentWorld()->FindGameObjectByID(*enttId);
		//
		// auto& world = GameStatics::GetCurrentWorld();
		//
		// if (gameObject->HasComponent<TransformComponent>())
		// {
		// 	auto& transform = gameObject->GetComponent<TransformComponent>();
		//
		// 	*position = transform.Position;
		// }

	}

	bool Engine_InputPressed(int32_t* keycode)
	{
		return Input::KeyPressed(static_cast<KeyCodes>(*keycode));
	}

	static void InitInternalCalls()
	{
		ADD_MONO_INTERNAL_CALL(Transform_SetPosition);
		ADD_MONO_INTERNAL_CALL(Transform_GetPosition);
		ADD_MONO_INTERNAL_CALL(Engine_InputPressed);

		spdlog::info("Script Engine internal Calls Init");
	}

	
}
