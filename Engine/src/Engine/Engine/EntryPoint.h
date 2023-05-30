#pragma once
#include "Application.h"


#include "optick.config.h"
#include "optick.h"
#include "Debug/Logger.h"


extern Polyboid::Application* Polyboid::CreateApplication();


 int main(int argc, char** argv)
{

    Polyboid::Logger::Init();
    Polyboid::Logger::GetLogger()->info("Loagger init");
    Polyboid::Application *app = Polyboid::CreateApplication();

    app->Run();
	delete app;

    spdlog::info("Allocation Count: {}", Polyboid::EngineMemoryManager::GetAllocationCount());
    spdlog::info("Free Count: {}", Polyboid::EngineMemoryManager::GetFreeCount());
    spdlog::warn("Memory not freed {}", Polyboid::EngineMemoryManager::GetAllocationCount() - Polyboid::EngineMemoryManager::GetFreeCount());



    //OPTICK_SHUTDOWN();

    
}