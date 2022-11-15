#pragma once

#include <GLFW/glfw3.h>
#include "Core.hpp"

namespace Cobra
{
    class COBRA_API Window
    {
        private:
            int width, height;
            const char* title;
        
        protected:
            GLFWwindow* screen;
        
        public:
            Window();
            Window(GLint Width, GLint Height);
            Window(GLint Width, GLint Height, const char* Title);
            ~Window();

            bool Display();
            void CreateWindow();
            GLFWwindow* GetScreen() const;
    };
}
