#pragma once

#include "EditorWindow.h"
#include <string>


struct ImGuiInputTextCallbackData;

namespace Polyboid 
{

	class ContentBrowserWindow : public EditorWindow
	{
	public:
		ContentBrowserWindow(const std::string&  name);
		virtual ~ContentBrowserWindow();

	private:

		int OnCreateFolder(ImGuiInputTextCallbackData* data);

		// Inherited via EditorWindow
		virtual void RenderImgui() override;

		virtual void Update(float ts) override;

	};

}
