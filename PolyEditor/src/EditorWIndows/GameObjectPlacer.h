#pragma once

#include "EditorWindow.h"
#include <string>
#include <vector>


namespace Polyboid 
{
	enum class GameObjectsType
	{
		EMPTY,
	};

	class GameObjectPlacer : public EditorWindow
	{

	private:
		std::vector<GameObjectsType> m_GameObjects;
	public:


		GameObjectPlacer(const std::string& name);
		virtual ~GameObjectPlacer();
		

	public:

		// Inherited via EditorWindow
		virtual void RenderImgui() override;

		virtual void Update(float ts) override;

	};
}
