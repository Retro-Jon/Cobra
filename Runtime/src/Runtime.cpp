#include "Cobra.hpp"

int main()
{
    Cobra::Window main;
    Cobra::Input input(main.GetScreen());

    Cobra::Component* root = new Cobra::Component("Root", "scripts/main.lua");

    while (!main.Display())
    {
        
    }

    delete root;

    return 0;
}