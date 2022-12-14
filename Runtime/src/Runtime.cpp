#include "Cobra.hpp"
#include <iostream>

int main()
{
    Cobra::PopulateMath();
    Cobra::Window main("settings.lua");
    
    Cobra::CreateObject("main.lua");

    Cobra::CalculateElapsedTime();

    while (!main.Display())
    {
        Cobra::CalculateElapsedTime();
        main.SetTitle(("Cobra FPS: " + std::to_string(1 / Cobra::ElapsedTime)).c_str());
        for (std::pair<int, Cobra::Object*> o : Cobra::objects)
        {
            o.second->Logic();
        }
    }

    Cobra::DeleteAllObjects();

    return 0;
}