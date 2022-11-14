#pragma once
#include "Application.h"
#include "Debug/Profiler.h"

extern Polyboid::Application* Polyboid::CreateApplication();


int main(int argc, char** argv)
{
    POLYBOID_PROFILE_BEGIN_SESSION("Startup", "BoidProfile-Startup.json");
    Polyboid::Application *app = Polyboid::CreateApplication();
    POLYBOID_PROFILE_END_SESSION();

    POLYBOID_PROFILE_BEGIN_SESSION("RunTime", "BoidProfile-Runtime.json");
    app->Run();
    POLYBOID_PROFILE_END_SESSION();


    POLYBOID_PROFILE_BEGIN_SESSION("Shutdown", "BoidProfile-Shutdown.json");
	delete app;
    POLYBOID_PROFILE_END_SESSION();
    
}