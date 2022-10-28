#pragma once

#include "EditorWindow.h"
#include <string>


namespace Polyboid 
{
	class WorldOutlinerWindow : public EditorWindow
	{
	public:
		WorldOutlinerWindow(const std::string& name);
		virtual ~WorldOutlinerWindow();

		// Inherited via EditorWindow
		virtual void RenderImgui() override;

		virtual void Update(float ts) override;

	};

}