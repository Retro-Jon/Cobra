#pragma once

#include "Core.hpp"
#include <string>
#include <iostream>

namespace Engine
{
    class Component;
    
    class API Application
    {
        private:
            bool running;
            std::string title;
        
        protected:
            Component* Root;
        
        public:
            Application();
            ~Application();

            void Run();
            void Test() const;
            int Quit();

            void SetDebug(bool);
            bool GetDebug();
            std::string GetTitle() const;
    };

    Application* CreateApplication();
}