#pragma once


#include <entt/entt.hpp>
#include "Engine/Engine/Base.h"

namespace Polyboid
{

	class GameObject;

	class World
	{
	public:
		entt::registry m_Registry;

	public:
		World(std::string name);

		static Ref<World> Create(const std::string& name);

		GameObject CreateGameObject(const std::string& name);


	private:
	
		std::string m_Name = "Untitled";

	};

}


