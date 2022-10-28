#include "EditorApp.h"


#include "imgui.h"
#include "Engine/Engine/Application.h"
#include "EditorLayer.h"


#define debugBreak() asm { int 3 }


namespace Polyboid
{

	EditorApp::EditorApp()
	{
        AddLayer(new EditorLayer("EditorLayer"));
	}



	Application* CreateApplication()
	{
		return new EditorApp();
	}
}

