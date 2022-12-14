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
            int screen_width, screen_height, resolution, pixel_scale, fov;
            const char* title;
            unsigned int FrameBufferOffScreen;
            std::map<std::string, Pos> cameras;
            std::string current_camera;
            std::vector<Sector> sectors;
            int current_sector;
            int running;
        
        protected:
            GLFWwindow* screen;
        
        public:
            Window(int Width = 160, int Height = 120, int Resolution = 1, int Pixel_Scale = 4, int Fov = 200, const char* Title = "Cobra");
            Window(std::string ScriptPath = "");

            void Init(int Width = 160, int Height = 120, int Resolution = 1, int Pixel_Scale = 4, int Fov = 200, const char* Title = "Cobra");

            ~Window();

            bool SwitchActiveCamera(int camera = 0);
            void MoveActiveCamera(Pos force = (Pos){.x = 0, .y = 0, .z = 0, .horizontal = 0, .vertical = 0});
            void MoveCamera(std::string name = "", Pos force = (Pos){.x = 0, .y = 0, .z = 0, .horizontal = 0, .vertical = 0});
            void PushCamera(std::string name = "", Pos force = (Pos){.x = 0, .y = 0, .z = 0, .horizontal = 0, .vertical = 0});
            void CreateNewCamera(std::string name, Pos position = (Pos){.x = 0, .y = 0, .z = 0, .horizontal = 0, .vertical = 0});
            int GetCameraCount();

            void RenderView();

            void ClipBehindCamera(double& x1, double& y1, double& z1, const double& x2, const double& y2, const double& z2);
            void DrawWall(double x1, double x2, double t1, double t2, double b1, double b2, int r, int g, int b);
            void Pixel(int x, int y, int r = 255, int g = 255, int b = 255);
            bool Display();
            void CreateWindow();
            void SetTitle(const char* Title);
            void End();
            GLFWwindow* GetScreen() const;
    };

    COBRA_API extern Window* window;

    void key_callback(GLFWwindow* w, int key, int scancode, int action, int mods);
}
