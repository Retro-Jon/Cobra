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
        // main.SetTitle(("FPS: " + std::to_string(1000 / Cobra::ElapsedTime)).c_str());
        for (std::pair<int, Cobra::Object*> o : Cobra::objects)
        {
            o.second->Logic();
        }

        Cobra::CalculateElapsedTime();
    }

    Cobra::DeleteAllObjects();

    return 0;
}