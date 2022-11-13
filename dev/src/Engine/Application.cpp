#include "Application.hpp"
#include "Component.hpp"
#include "General.hpp"

#include <iostream>

namespace Engine
{
    Application::Application()
    {
        title = "App";
        running = true;
        Root = new Component("Root", "scripts/main.lua");
        Log("Created!");
    }

    Application::~Application()
    {
        delete Root;
    }

    void Application::Run()
    {
        while (running)
        {
            Root->Logic();
        }
    }

    void Application::Test() const
    {
        Log("Application Test");
    }

    int Application::Quit()
    {
        running = false;
        return 0;
    }

    std::string Application::GetTitle() const
    {
        return title;
    }
}
