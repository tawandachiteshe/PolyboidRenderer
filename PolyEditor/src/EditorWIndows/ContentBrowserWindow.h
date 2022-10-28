#pragma once

#include "EditorWindow.h"
#include <string>

namespace Polyboid 
{

	class ContentBrowserWindow : public EditorWindow
	{
	public:
		ContentBrowserWindow(const std::string&  name);
		virtual ~ContentBrowserWindow();

	private:


		// Inherited via EditorWindow
		virtual void RenderImgui() override;

		virtual void Update(float ts) override;

	};

}