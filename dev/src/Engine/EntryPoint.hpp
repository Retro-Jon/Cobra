#pragma once

#include "General.hpp"
#include "Application.hpp"
#include <iostream>

#ifdef PLATFORM_WINDOWS

int main(int argc, char** argv)
{
    Engine::app = new Engine::Application();
    
    Engine::app->Run();

    delete Engine::app;
}

#endif