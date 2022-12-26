#include "Cobra.hpp"
#include <iostream>

int main()
{
    Cobra::PopulateMath();
    Cobra::window = new Cobra::Window("settings.lua");
    
    Cobra::CreateObject("main.lua");

    Cobra::CalculateElapsedTime();

    while (!Cobra::window->Display())
    {
        for (std::pair<int, Cobra::Object*> o : Cobra::objects)
        {
            o.second->Logic();
        }

        Cobra::CalculateElapsedTime();
    }

    Cobra::DeleteAllObjects();

    return 0;
}