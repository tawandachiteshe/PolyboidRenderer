#pragma once
#include "Application.h"


extern Polyboid::Application* Polyboid::CreateApplication();


int main(int argc, char** argv)
{
    Polyboid::Application *app = Polyboid::CreateApplication();
    app->Run();
    delete app;
}