#pragma once

#include "Core.hpp"
#include <GLFW/glfw3.h>

namespace Cobra
{
    class COBRA_API Input
    {
        private:
            static void KeyCallBack(GLFWwindow* screen, int key, int scancode, int action, int mods);
        
        public:
            Input(GLFWwindow* screen);
    };
}
