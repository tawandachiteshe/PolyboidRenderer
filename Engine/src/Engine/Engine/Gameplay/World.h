#pragma once
#include <vector>
#include <string>

#include "Engine/Engine/ECS/GameObject.h"

namespace Polygon
{
	class World
	{
	public:
		World(const std::string& name = "World");
		void OnBeginPlay();
		void OnUpdate(float ts);
		void OnEndPlay();

	private:
		std::string name;

		//find a better data structure for this:::
		std::vector<Polyboid::GameObject> m_GameObjects;
		
	};
}
