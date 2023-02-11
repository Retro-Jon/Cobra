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
            int w, h, r, p, fov, fps;

            std::string t;

            lua_State* settings = luaL_newstate();

            luaL_dofile(settings, ("scripts/" + ScriptPath).c_str());

            lua_getglobal(settings, "Settings");
            lua_pushstring(settings, "Window");

            lua_gettable(settings, -2);

            w = LuaGetTableNumberValue<int>(settings, "Width");
            h = LuaGetTableNumberValue<int>(settings, "Height");
            r = LuaGetTableNumberValue<int>(settings, "Resolution");
            p = LuaGetTableNumberValue<int>(settings, "PixelScale");
            t = LuaGetTableStringValue(settings, "Title");

            lua_pop(settings, 1);
            lua_pushstring(settings, "Rendering");
            lua_gettable(settings, -2);

            fov = LuaGetTableNumberValue<int>(settings, "Fov");
            fps = LuaGetTableNumberValue<int>(settings, "FPS");

            Init(w, h, r, p, fov, fps, t.c_str());

            lua_close(settings);
        } else {
            Init(160, 120, 1, 4, 200, 30, "Cobra");
        }
    }

    Window::Window(int Width, int Height, int Resolution, int Pixel_Scale, int Fov, int FPS, const char* Title)
    {
        Init(Width, Height, Resolution, Pixel_Scale, Fov, FPS, Title);
    }

    Window::~Window()
    {
        window = nullptr;
    }

    void Window::Init(int Width, int Height, int Resolution, int Pixel_Scale, int Fov, int FPS, const char* Title)
    {
        window = this;
        
        resolution = Resolution;
        screen_width = Width;
        screen_height = Height;
        pixel_scale = Pixel_Scale / resolution;

        fov = Fov * Resolution;
        fps = FPS;

        if (Resolution > Pixel_Scale)
        {
            std::cout << "Resolution can't be bigger than Pixel_Scale." << std::endl;
            exit(1);
        }
        
        title = Title;

        CreateWindow();
        Cobra::renderer = new Cobra::Renderer(fov);
        rendering = true;
        return;
    }

    void Window::CreateWindow()
    {
        glfwInit();
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        screen = glfwCreateWindow((screen_width * resolution) * pixel_scale, (screen_height * resolution) * pixel_scale, title, NULL, NULL);

        glfwSetKeyCallback(screen, key_callback);
        glfwSetWindowSizeCallback(screen, size_callback);
        glfwSetWindowMaximizeCallback(screen, maximize_callback);

        glfwSwapInterval(1);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        glfwMakeContextCurrent(screen);
        glOrtho(0, screen_width * resolution, 0, screen_height * resolution, -1.0f, 1.0f);

        running = true;
    }

    void Window::SetTitle(const char* Title)
    {
        glfwSetWindowTitle(screen, Title);
        title = Title;
    }

    void Window::End()
    {
        running = true;
    }

    int Window::GetWidth()
    {
        return screen_width;
    }

    int Window::GetHeight()
    {
        return screen_height;
    }

    int Window::GetResolution()
    {
        return resolution;
    }

    bool Window::Display()
    {
        static double rvt = 0; // Render View Timer

        rvt += fps / ElapsedTime;
        
        if (rvt > 1)
        {
            if (IsRendering())
            {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glPointSize(pixel_scale);
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
        pixel_scale = ((width * (resolution * 0.0025)) + (height * 0.0025)) / 2;
        glViewport(0, 0, (width * resolution), (height * resolution));
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
