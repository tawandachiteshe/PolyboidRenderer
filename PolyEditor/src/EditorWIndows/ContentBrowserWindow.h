#pragma once

#include <future>

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

		std::vector<std::future<void>> m_Futures;
		void CheckIfMeshIsLoaded();

		int OnCreateFolder(ImGuiInputTextCallbackData* data);

		// Inherited via EditorWindow
		virtual void RenderImgui() override;

		virtual void Update(float ts) override;

	};

}
