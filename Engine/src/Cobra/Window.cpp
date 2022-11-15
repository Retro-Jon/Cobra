#include "headers/Window.hpp"

namespace Cobra
{
    Window::Window()
    {
        width = 1080;
        height = 720;
        title = "Cobra";
        CreateWindow();
    }

    Window::Window(GLint Width, GLint Height)
    {
        width = Width;
        height = Height;
        title = "Cobra";
        CreateWindow();
    }

    Window::Window(GLint Width, GLint Height, const char* Title)
    {
        width = Width;
        height = Height;
        title = Title;
        CreateWindow();
    }

    void Window::CreateWindow()
    {
        glfwInit();
        screen = glfwCreateWindow(width, height, title, NULL, NULL);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glfwSwapInterval(1);

        glfwMakeContextCurrent(screen);
    }

    Window::~Window()
    {
        
    }

    bool Window::Display()
    {
        glfwSwapBuffers(screen);
        glfwPollEvents();

        return glfwWindowShouldClose(screen);
    }

    GLFWwindow* Window::GetScreen() const
    {
        return screen;
    }
}
