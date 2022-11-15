#include "headers/Input.hpp"
#include <iostream>

namespace Cobra
{
    Input::Input(GLFWwindow* screen)
    {
        glfwSetKeyCallback(screen, KeyCallBack);
    }

    void KeyCallBack(GLFWwindow* screen, int key, int scancode, int action, int mods)
    {
        if (action == GLFW_PRESS)
        {
            std::cout << key << std::endl;
        } else if (action == GLFW_RELEASE)
        {
            std::cout << key << std::endl;
        }
    }
}