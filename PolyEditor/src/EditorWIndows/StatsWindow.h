#pragma once
#include "EditorWindow.h"
#include "imgui.h"
#include "Engine/Engine/Base.h"


namespace Polyboid
{
	class Texture2D;

	class StatsWindow : public EditorWindow
	{
		ImTextureID m_Texture = nullptr;
		ImTextureID m_RedTexture = nullptr;
		ImTextureID m_GreenTexture = nullptr;
		ImTextureID m_BlueTexture = nullptr;

	public:
		StatsWindow();
		void RenderImgui() override;
		void Update(float ts) override;
		~StatsWindow() override;
	};
}
