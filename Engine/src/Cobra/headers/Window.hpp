#pragma once

#include <GLFW/glfw3.h>
#include "Core.hpp"
#include <iostream>
#include <vector>
#include <map>
#include "Sectors.hpp"
#include "General.hpp"

namespace Cobra
{
    class COBRA_API Window
    {
        private:
            int screen_width, screen_height, resolution, pixel_scale;
            const char* title;
            unsigned int FrameBufferOffScreen;
            std::map<std::string, Pos> cameras;
            std::string current_camera;
            std::vector<Sector> sectors;
            int current_sector;
            int running;
            std::pair<float, float>* UsedScreenSpace;
        
        protected:
            GLFWwindow* screen;
        
        public:
            Window(int Width = 160, int Height = 120, int Resolution = 1, int Pixel_Scale = 4, const char* Title = "Cobra");
            ~Window();

            bool SwitchActiveCamera(int camera = 0);
            void MoveActiveCamera(Pos force = (Pos){.x = 0, .y = 0, .z = 0, .angle = 0});
            void MoveCamera(std::string name = "", Pos force = (Pos){.x = 0, .y = 0, .z = 0, .angle = 0});
            void PushCamera(std::string name = "", Pos force = (Pos){.x = 0, .y = 0, .z = 0, .angle = 0});
            void CreateNewCamera(std::string name, Pos position = (Pos){.x = 0, .y = 0, .z = 0, .angle = 0});
            int GetCameraCount();

            void RenderView();

            void Pixel(int x, int y, int r = 255, int g = 255, int b = 255);
            bool Display();
            void CreateWindow();
            void End();
            GLFWwindow* GetScreen() const;
    };

    COBRA_API extern Window* window;

    void key_callback(GLFWwindow* w, int key, int scancode, int action, int mods);
}
