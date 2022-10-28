#pragma once

#include "EditorWindow.h"
#include <string>


namespace Polyboid 
{
	class GameObjectPlacer : public EditorWindow
	{
	public:


		GameObjectPlacer(const std::string& name);
		virtual ~GameObjectPlacer();

	public:
		// Inherited via EditorWindow
		virtual void RenderImgui() override;

		virtual void Update(float ts) override;

	};
}