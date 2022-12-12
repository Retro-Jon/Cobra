#include "Cobra.hpp"
#include <iostream>

int main()
{
    Cobra::PopulateMath();
    Cobra::Window main;
    Cobra::CreateObject("main.lua");

    while (!main.Display())
    {
        for (std::pair<int, Cobra::Object*> o : Cobra::objects)
        {
            o.second->Logic();
        }
    }

    Cobra::DeleteAllObjects();

    return 0;
}