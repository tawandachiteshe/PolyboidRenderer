#include "EditorApp.h"


#include "imgui.h"
#include "Engine/Engine/Application.h"
#include "EditorLayer.h"


#define debugBreak() asm { int 3 }


namespace Polyboid
{

	EditorApp::EditorApp()
	{
        AddLayer(EngineMemoryManager::AllocateMem<EditorLayer>("EditorLayer"));
	}



	Application* CreateApplication()
	{
		ApplicationSettings settings;
		CREATE_APPLICATION(EditorApp, app)
		return app;
	}
}

