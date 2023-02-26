#pragma once
#include "Application.h"


#include "optick.config.h"
#include "optick.h"


extern Polyboid::Application* Polyboid::CreateApplication();


int main(int argc, char** argv)
{
 //    OPTICK_APP("Polyboid Engine");
 //
	// OPTICK_SET_MEMORY_ALLOCATOR(
 //            [](size_t size) -> void* { return operator new(size); },
 //            [](void* p) { operator delete(p); },
 //            []() { /* Do some TLS initialization here if needed */ }
 //    );


    Polyboid::Application *app = Polyboid::CreateApplication();
    app->Run();
	delete app;



    //OPTICK_SHUTDOWN();

    
}