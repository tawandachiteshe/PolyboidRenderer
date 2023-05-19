#pragma once
#include "EditorWindow.h"
#include "Engine/Engine/Base.h"


namespace Polyboid
{
	class Texture;

	class StatsWindow : public EditorWindow
	{
		Ref<Texture> m_Texture = nullptr;
		Ref<Texture> m_RedTexture = nullptr;
		Ref<Texture> m_GreenTexture = nullptr;
		Ref<Texture> m_BlueTexture = nullptr;

	public:
		StatsWindow();
		void RenderImgui() override;
		void Update(float ts) override;
		~StatsWindow() override;
	};
}
