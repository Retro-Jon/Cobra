#pragma once

#include "Core.hpp"
#include <iostream>
#include <vector>
#include <map>
#include "Sectors.hpp"
#include "General.hpp"

#include <GLFW/glfw3.h>

namespace Cobra
{
    class COBRA_API Window
    {
        private:
            int screen_width, screen_height, fov, fps;
            const char* title;
            int current_sector;
            bool running;
            bool rendering;
        
        protected:
            GLFWwindow* screen;
        
        public:
            Window(int Width = 160, int Height = 120, int Fov = 200, int FPS = 30, const char* Title = "Cobra");
            Window(std::string ScriptPath = "");
            ~Window();

            void Init(int Width = 160, int Height = 120, int Fov = 200, int FPS = 30, const char* Title = "Cobra");
            bool IsRunning() {return running;}
            bool IsRendering() {return rendering;}
            void SetRendering(bool value) {rendering = value;}

            int GetWidth();
            int GetHeight();

            bool Display();
            void CreateWindow();
            void SetTitle(const char* Title);
            void SetSize(int width, int height);
            void ResetSize();
            void End();
            GLFWwindow* GetScreen() const;
    };

    COBRA_API extern Window* window;

    void key_callback(GLFWwindow* w, int key, int scancode, int action, int mods);
    void size_callback(GLFWwindow* w, int width, int height);
    void maximize_callback(GLFWwindow* w, int maximized);
}
