#pragma once


#include <entt/entt.hpp>
#include "Engine/Engine/Base.h"

namespace Polyboid
{
	class Camera;

	class GameObject;

	class World
	{
	public:
		entt::registry m_Registry;
		Ref<Camera> m_CurrentCamera;

	public:
		World(std::string name);

		static Ref<World> Create(const std::string& name);

		void Update(float ts);

		GameObject CreateGameObject(const std::string& name);

		void OnRender(const Ref<Camera>& camera);


	private:
	
		std::string m_Name = "Untitled";

	};

}


