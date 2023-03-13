#include "headers/Window.hpp"
#include "headers/Object.hpp"
#include "headers/General.hpp"
#include "headers/Renderer.hpp"

#include <algorithm>

namespace Cobra
{
    Window* window;

    Window::Window(std::string ScriptPath)
    {
        if (ScriptPath != "")
        {
            int w, h, r, p, fps;

            std::string t;

            lua_State* settings = luaL_newstate();

            luaL_dofile(settings, ("scripts/" + ScriptPath).c_str());

            lua_getglobal(settings, "Settings");
            lua_pushstring(settings, "Window");

            lua_gettable(settings, -2);

            w = LuaGetTableNumberValue<int>(settings, "Width");
            h = LuaGetTableNumberValue<int>(settings, "Height");
            t = LuaGetTableStringValue(settings, "Title");

            lua_pop(settings, 1);
            lua_pushstring(settings, "Rendering");
            lua_gettable(settings, -2);

            fps = LuaGetTableNumberValue<int>(settings, "FPS");

            Init(w, h, fps, t.c_str());

            lua_close(settings);
        } else {
            Init(160, 120, 30, "Cobra");
        }
    }

    Window::Window(int Width, int Height, int FPS, const char* Title)
    {
        Init(Width, Height, FPS, Title);
    }

    Window::~Window()
    {
        glfwDestroyWindow(screen);
        glfwTerminate();
    }

    // Set member variables and create renderer
    void Window::Init(int Width, int Height, int FPS, const char* Title)
    {
        window = this;
        
        screen_width = Width;
        screen_height = Height;

        fps = FPS;
        
        title = Title;

        CreateWindow();
        Cobra::renderer = new Cobra::Renderer();
        rendering = true;
        return;
    }

    // Create window instance
    void Window::CreateWindow()
    {
        glfwInit();
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        screen = glfwCreateWindow(screen_width, screen_height, title, NULL, NULL);

        glfwSetKeyCallback(screen, key_callback);
        glfwSetWindowSizeCallback(screen, size_callback);
        glfwSetWindowMaximizeCallback(screen, maximize_callback);

        glfwSwapInterval(1);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        glfwMakeContextCurrent(screen);
        glOrtho(0, screen_width, screen_height, 0, -1.0f, 1.0f);

        running = true;
    }

    void Window::SetTitle(const char* Title)
    {
        glfwSetWindowTitle(screen, Title);
        title = Title;
    }

    void Window::End()
    {
        running = false;
    }

    int Window::GetWidth()
    {
        return screen_width;
    }

    int Window::GetHeight()
    {
        return screen_height;
    }

    // Call renderer and regulate frame rate
    bool Window::Display()
    {
        static double rvt = 0; // Render View Timer

        rvt += fps * ElapsedTime;
        
        if (rvt > 1)
        {
            if (IsRendering())
            {
                glClearColor(0, 0, 0, 1);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                renderer->RenderView();
                glfwSwapBuffers(screen);
            }

            rvt = 0;
        }

        glfwPollEvents();
        
        if (!running)
            glfwSetWindowShouldClose(screen, GL_TRUE);
        
        return glfwWindowShouldClose(screen);
    }

    GLFWwindow* Window::GetScreen() const
    {
        return screen;
    }

    void Window::SetSize(int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    void Window::ResetSize()
    {
        glfwSetWindowSize(GetScreen(), screen_width, screen_height);
    }

    void key_callback(GLFWwindow* w, int key, int scancode, int action, int mods)
    {
        for (std::pair<int, Cobra::Object*> o : Cobra::objects)
        {
            o.second->KeyInput(key, action);
        }
    }

    void size_callback(GLFWwindow* w, int width, int height)
    {
        if (!glfwGetWindowAttrib(w, GLFW_MAXIMIZED))
            window->ResetSize();
    }

    void maximize_callback(GLFWwindow* w, int maximized)
    {
        int width, height;
        glfwGetWindowSize(w, &width, &height);
        window->SetSize(width, height);
    }
}
