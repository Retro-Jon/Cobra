#include "headers/Window.hpp"
#include "headers/Object.hpp"
#include "headers/General.hpp"
#include "headers/AssetLoader.hpp"

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

        current_camera = "";
        current_sector = 0;

        sectors.push_back(assetloader->LoadSector("level.sect", window->GetWidth() * window->GetResolution()));

        sector_order.push_back(sectors.size() - 1);

        CreateWindow();
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
        rendering = true;
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

    Window::~Window()
    {
        cameras.clear();
        sectors.clear();
        sector_order.clear();
    }

    bool Window::SwitchActiveCamera(int camera)
    {
        if (camera < cameras.size())
        {
            current_camera = camera;
            return true;
        }

        return false;
    }

    void Window::MoveActiveCamera(Pos force)
    {
        MoveCamera(current_camera, force);
    }

    void Window::MoveCamera(std::string name, Pos force)
    {
        cameras[name] = force;
    }

    void Window::PushCamera(std::string name, Pos force)
    {
        Pos* cc = &cameras[name];

        cc->horizontal += force.horizontal * ElapsedTime;
        cc->vertical += force.vertical * ElapsedTime;

        Clamp(cc->horizontal, 0, 360);
        Clamp(cc->vertical, 0, 360);

        double COS = M.cos[(int)cc->horizontal] * ElapsedTime;
        double SIN = M.sin[(int)cc->horizontal] * ElapsedTime;
        
        if (force.x != 0)
        {
            if (M.tan[(int)cc->horizontal] <= 90 || M.tan[(int)cc->horizontal] >= 270)
            {
                cc->x += force.x * COS;
                cc->y -= force.x * SIN;
            } else {
                cc->x -= force.x * COS;
                cc->y += force.x * SIN;
            }
        }
        if (force.y != 0)
        {
            cc->x += force.y * SIN;
            cc->y += force.y * COS;
        }

        cc->z += force.z * ElapsedTime;
    }

    void Window::CreateNewCamera(std::string name, Pos position)
    {
        cameras.insert(std::pair<std::string, Pos>(name, position));
        
        if (current_camera == "")
            current_camera = name;
    }

    int Window::GetCameraCount()
    {
        return cameras.size();
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

    void Window::BubbleSortSectors(double z)
    {
        int limit = 0;
        // distance
        for (int s = 0; s < sector_order.size(); s++)
        {
            for (int w = 0; w < sector_order.size() - limit; w++)
            {
                if (w + 1 < sector_order.size() - limit)
                {
                    if (sectors[sector_order[w]].distance < sectors[sector_order[w + 1]].distance)
                    {
                        int tmp = sector_order[w];
                        sector_order[w] = sector_order[w + 1];
                        sector_order[w + 1] = tmp;
                    }
                }
            }
            limit++;
        }

        limit = 0;

        // z direction
        for (int s = 0; s < sector_order.size(); s++)
        {
            for (int w = 0; w < sector_order.size() - limit; w++)
            {
                if (w + 1 < sector_order.size() - limit)
                {
                    if (sectors[sector_order[w]].top > sectors[sector_order[w + 1]].top && z > sectors[sector_order[w]].bottom)
                    {
                        int tmp = sector_order[w];
                        sector_order[w] = sector_order[w + 1];
                        sector_order[w + 1] = tmp;
                    } else if (sectors[sector_order[w]].bottom < sectors[sector_order[w + 1]].bottom && z < sectors[sector_order[w]].top)
                    {
                        int tmp = sector_order[w];
                        sector_order[w] = sector_order[w + 1];
                        sector_order[w + 1] = tmp;
                    }
                }
            }
            limit++;
        }
    }

    void Window::RenderView()
    {
        const double multiplier = screen_width / fov;
        static Pos last_cc;
        
        Pos cc = cameras[current_camera];

        if (cc.x != last_cc.x || cc.y != last_cc.y || cc.z != last_cc.z)
        {
            if (cc.horizontal != last_cc.horizontal || cc.vertical != last_cc.vertical)
                BubbleSortSectors(cc.z);
            
            // Draw Sectors
            for (int index : sector_order)
            {
                Sector cs = sectors[index];
                sectors[index].distance = 0;

                if (cc.z < cs.bottom)
                    sectors[index].surface = 1; // Bottom
                else if (cc.z > cs.top)
                    sectors[index].surface = 2; // Top
                else
                    sectors[index].surface = 0; // None

                for (int loop = 0; loop < 2; loop++)
                {
                    for (int w = 0; w < cs.wall_count; w++)
                    {
                        SectorWall wall;

                        wall.p1.x = cs.walls[w].p1.x - cc.x;
                        wall.p1.y = cs.walls[w].p1.y - cc.y;

                        wall.p2.x = cs.walls[w].p2.x - cc.x;
                        wall.p2.y = cs.walls[w].p2.y - cc.y;

                        wall.wall_color = cs.walls[w].wall_color;

                        double wx[4], wy[4], wz[4];
                        double COS = M.cos[(int)cc.horizontal], SIN = M.sin[(int)cc.horizontal];

                        // swap surface

                        if (cs.view != Inverted)
                        {
                            if (loop == 0)
                            {
                                SectorPoint swp = wall.p1;
                                wall.p1 = wall.p2;
                                wall.p2 = swp;
                            }
                        } else {
                            if (loop == 1)
                            {
                                SectorPoint swp = wall.p1;
                                wall.p1 = wall.p2;
                                wall.p2 = swp;
                            }
                        }

                        // World Positions
                        // X
                        wx[0] = wall.p1.x * COS - wall.p1.y * SIN;
                        wx[1] = wall.p2.x * COS - wall.p2.y * SIN;
                        // Y
                        wy[0] = wall.p1.y * COS + wall.p1.x * SIN;
                        wy[1] = wall.p2.y * COS + wall.p2.x * SIN;
                        wy[2] = wall.p1.y * COS + wall.p1.x * SIN;
                        wy[3] = wall.p2.y * COS + wall.p2.x * SIN;

                        sectors[index].distance = dist(0, 0, (wx[0] + wx[1]) / 2, (wy[0] + wy[1]) / 2); // Store Wall Distance

                        // Z
                        wz[0] = cs.bottom - cc.z + (((cc.vertical - 90) * wy[0]) / 32.00);
                        wz[1] = cs.bottom - cc.z + (((cc.vertical - 90) * wy[1]) / 32.00);
                        wz[2] = cs.top - cc.z + (((cc.vertical - 90) * wy[2]) / 32.00);
                        wz[3] = cs.top - cc.z + (((cc.vertical - 90) * wy[3]) / 32.00);

                        // Screen Positions
                        // X
                        wx[0] = wx[0] * fov / wy[0] + (screen_width * resolution / 2);
                        wx[1] = wx[1] * fov / wy[1] + (screen_width * resolution / 2);

                        // Y
                        wy[0] = wz[0] * fov / wy[0] + (screen_height * resolution / 2);
                        wy[1] = wz[1] * fov / wy[1] + (screen_height * resolution / 2);
                        wy[2] = wz[2] * fov / wy[2] + (screen_height * resolution / 2);
                        wy[3] = wz[3] * fov / wy[3] + (screen_height * resolution / 2);
                        
                        if (wy[0] < 1 && wy[1] < 1) break;

                        if (wy[0] < 1)
                        {
                            ClipBehindCamera(wx[0], wy[0], wz[0], wx[1], wy[1], wz[1]); // bottom
                            ClipBehindCamera(wx[2], wy[2], wz[2], wx[3], wy[3], wz[3]); // top
                        }
                        
                        if (wy[1] < 1)
                        {
                            ClipBehindCamera(wx[1], wy[1], wz[1], wx[0], wy[0], wz[0]); // bottom
                            ClipBehindCamera(wx[3], wy[3], wz[3], wx[2], wy[2], wz[2]); // top
                        }
                        
                        DrawWall(wx[0], wx[1], wy[0], wy[1], wy[2], wy[3], sectors[index].surface, cs.surf_points, cs.view, wall.wall_color, cs.top_color, cs.bottom_color);
                    }
                    sectors[index].surface *= -1;
                }
            }
        }
    }

    void Window::ClipBehindCamera(double& x1, double& y1, double& z1, const double& x2, const double& y2, const double& z2)
    {
        float da = y1;  // distance plane a
        float db = y2;  // distance plane b
        float d = da - db;

        if (d < 1) d = 1;

        float s = da / d; // intersection factor

        x1 += s * (x2 - (x1));
        y1 += s * (y2 - (y1));
        z1 += s * (z2 - (z1));
    }

    void Window::DrawWall(double x1, double x2, double t1, double t2, double b1, double b2, int surface, int* points, int view, Color wc, Color tc, Color bc)
    {
        // differences
        double dyb = b2 - b1;
        double dyt = t2 - t1;

        double dx = x2 - x1;

        if (dx == 0) dx = 1;

        glBegin(GL_POINTS);
        
        int xs = (int)x1, xf = (int)x2;

        if (x1 < 0) x1 = 0;
        if (x2 < 0) x2 = 0;
        if (x1 > screen_width) x1 = screen_width;
        if (x2 > screen_width) x2 = screen_width;

        for (int x = xs; x < xf; x++)
        {
            if (x >= 0 && x < screen_width)
            {
                int y2 = dyb * (x - xs) / dx + b1;
                int y1 = dyt * (x - xs) / dx + t1;

                // Surface

                // Save
                if (view != Hollow)
                {
                    if (surface == 1)           // Bottom
                    {
                        points[x] = y1;
                        continue;
                    } else if (surface == 2)    // Top
                    {
                        points[x] = y2;
                        continue;
                    }
                }
                
                // Draw
                if (view != Hollow)
                {
                    if (surface == -1)          // Bottom
                    {
                        glColor3ub(bc.r, bc.g, bc.b);
                        for (int y = points[x]; y < y1; y++)
                            glVertex2i(x, y);
                    } else if (surface == -2)   // Top
                    {
                        glColor3ub(tc.r, tc.g, tc.b);
                        for (int y = y2; y < points[x]; y++)
                            glVertex2i(x, y);
                    }
                }

                // Normal Wall
                glColor3ub(wc.r, wc.g, wc.b);

                Clamp(y1, 0, screen_height, false);
                Clamp(y2, 0, screen_height, false);

                int y = y1, yt = y2, dir = 1;

                if (y1 > y2)
                {
                    y = y2;
                    yt = y1;
                }

                for (y; y < yt; y++)
                    glVertex2i(x, y);
            }
        }
        glEnd();
    }

    void Window::Pixel(int x, int y, int r, int g, int b)
    {
        glColor3ub(r, g, b);
        glBegin(GL_POINTS);
        glVertex2i(x, y);
        glEnd();
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
                RenderView();
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
