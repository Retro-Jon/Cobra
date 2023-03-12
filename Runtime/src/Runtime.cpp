#include "Cobra.hpp"
#include <iostream>

#define APP

int main()
{
    Cobra::assetloader = new Cobra::AssetLoader();
    Cobra::window = new Cobra::Window("settings.lua");

    Cobra::renderer->SetSectors(Cobra::assetloader->LoadSectors("level.sect", Cobra::window->GetWidth()));
    
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

    Cobra::assetloader->SaveSectors("test.sect", Cobra::renderer->GetSectors());

    Cobra::DeleteAllObjects();
    delete Cobra::renderer;
    delete Cobra::window;

    return 0;
}