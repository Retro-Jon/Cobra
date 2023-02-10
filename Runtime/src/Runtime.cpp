#include "Cobra.hpp"
#include <iostream>

#define APP

int main()
{
    Cobra::PopulateMath();
    Cobra::window = new Cobra::Window("settings.lua");
    Cobra::assetloader = new Cobra::AssetLoader();
    
    Cobra::CreateObject("main.lua");

    Cobra::CalculateElapsedTime();

    int delete_timer = 10000;
    
    while (!Cobra::window->Display())
    {
        for (std::pair<int, Cobra::Object*> o : Cobra::objects)
        {
            o.second->Logic();
        }

        if (delete_timer <= 0)
        {
            while (Cobra::DeletionQueue.size() > 0)
            {
                delete Cobra::DeletionQueue.front();
                Cobra::DeletionQueue.pop();
            }
            delete_timer = 10000;
        }

        delete_timer--;

        Cobra::CalculateElapsedTime();
    }

    Cobra::DeleteAllObjects();
    delete Cobra::window;

    return 0;
}