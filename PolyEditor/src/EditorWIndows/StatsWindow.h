#pragma once
#include "EditorWindow.h"


namespace Polyboid
{
	class StatsWindow : public EditorWindow
	{
	public:
		StatsWindow();
		void RenderImgui() override;
		void Update(float ts) override;
		~StatsWindow() override;
	};
}
