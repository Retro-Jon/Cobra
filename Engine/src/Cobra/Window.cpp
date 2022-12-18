#include "headers/Window.hpp"
#include "headers/Object.hpp"
#include "headers/General.hpp"

#include <algorithm>

void dumpstack (lua_State *L) {
  int top=lua_gettop(L);
  for (int i=1; i <= top; i++) {
    printf("%d\t%s\t", i, luaL_typename(L,i));
    switch (lua_type(L, i)) {
      case LUA_TNUMBER:
        printf("%g\n",lua_tonumber(L,i));
        break;
      case LUA_TSTRING:
        printf("%s\n",lua_tostring(L,i));
        break;
      case LUA_TBOOLEAN:
        printf("%s\n", (lua_toboolean(L, i) ? "true" : "false"));
        break;
      case LUA_TNIL:
        printf("%s\n", "nil");
        break;
      default:
        printf("%p\n",lua_topointer(L,i));
        break;
    }
  }
}

namespace Cobra
{
    Window* window;

    Window::Window(std::string ScriptPath)
    {
        if (ScriptPath != "")
        {
            int w, h, r, p, f;

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

            f = LuaGetTableNumberValue<int>(settings, "Fov");
            Init(w, h, r, p, f, t.c_str());

            lua_close(settings);
        } else {
            Init(160, 120, 1, 4, 200, "Cobra");
        }
    }

    Window::Window(int Width, int Height, int Resolution, int Pixel_Scale, int Fov, const char* Title)
    {
        Init(Width, Height, Resolution, Pixel_Scale, Fov, Title);
    }

    void Window::Init(int Width, int Height, int Resolution, int Pixel_Scale, int Fov, const char* Title)
    {
        running = true;
        window = this;
        
        resolution = Resolution;
        screen_width = Width;
        screen_height = Height;
        pixel_scale = Pixel_Scale / resolution;

        fov = Fov * Resolution;

        if (Resolution > Pixel_Scale)
            std::cout << "Resolution can't be bigger than Pixel_Scale." << std::endl;
        
        title = Title;

        current_camera = "";
        current_sector = 0;

        int count = 0;

        for (int sx = -8; sx <= 8; sx += 8)
        {
            Color c;

            switch (count)
            {
                case 0:
                    c = {.r = 255, .g = 0, .b = 0};
                    break;
                case 1:
                    c = {.r = 0, .g = 255, .b = 0};
                    break;
                case 2:
                    c = {.r = 0, .g = 0, .b = 255};
                    break;
            }

            Sector s;
            s.wall_count = 4;
            s.walls = new SectorWall[4];
            SectorPoint points[4];

            points[0] = (SectorPoint){.x = sx + -2, .y = -2};
            points[1] = (SectorPoint){.x = sx + 2, .y = -2};
            points[2] = (SectorPoint){.x = sx + 2, .y = 2};
            points[3] = (SectorPoint){.x = sx + -2, .y = 2};

            bool shaded = false;
            
            for (int w = 0; w < s.wall_count; w++)
            {
                s.walls[w].wall_color = c;

                if (shaded)
                    s.walls[w].wall_color = {.r = c.r - 100, .g = c.g - 100, .b = c.b - 100};
                
                Clamp(s.walls[w].wall_color.r, 0, 255, false);
                Clamp(s.walls[w].wall_color.g, 0, 255, false);
                Clamp(s.walls[w].wall_color.b, 0, 255, false);
                
                shaded = !shaded;

                s.walls[w].p1 = points[w];

                if (w +1 < s.wall_count)
                    s.walls[w].p2 = points[w + 1];
                else
                    s.walls[w].p2 = points[0];
            }

            s.top = 2;
            s.bottom = 0;

            sectors.push_back(s);

            count++;
            if (count > 2) count = 0;
        }

        CreateWindow();
    }

    void Window::CreateWindow()
    {
        glfwInit();
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        screen = glfwCreateWindow((screen_width * resolution) * pixel_scale, (screen_height * resolution) * pixel_scale, title, NULL, NULL);

        glfwSetKeyCallback(screen, key_callback);

        glfwSwapInterval(1);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        glfwMakeContextCurrent(screen);
        glOrtho(0, screen_width * resolution, 0, screen_height * resolution, -1.0f, 1.0f);
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

    void Window::RenderView()
    {
        const double multiplier = screen_width / fov;
        
        Pos cc = cameras[current_camera];

        // Sort Sectors
        for (int s = 0; s < sectors.size(); s++)
        {
            for (int w = 0; w < sectors.size(); w++)
            {
                if (sectors[w].distance < sectors[w + 1].distance)
                {
                    Sector tmp = sectors[w];
                    sectors[w] = sectors[w + 1];
                    sectors[w + 1] = tmp;
                }
            }
        }
        
        for (Sector cs : sectors)
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

                // World Positions
                // X
                wx[0] = wall.p1.x * COS - wall.p1.y * SIN;
                wx[1] = wall.p2.x * COS - wall.p2.y * SIN;
                // Y
                wy[0] = wall.p1.y * COS + wall.p1.x * SIN;
                wy[1] = wall.p2.y * COS + wall.p2.x * SIN;
                wy[2] = wall.p1.y * COS + wall.p1.x * SIN;
                wy[3] = wall.p2.y * COS + wall.p2.x * SIN;
                // Z
                wz[0] = cs.bottom - cc.z + ((cc.vertical * wy[0]) / 32.00);
                wz[1] = cs.bottom - cc.z + ((cc.vertical * wy[1]) / 32.00);
                wz[2] = cs.top - cc.z + ((cc.vertical * wy[2]) / 32.00);
                wz[3] = cs.top - cc.z + ((cc.vertical * wy[3]) / 32.00);

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

                DrawWall(wx[0], wx[1], wy[0], wy[1], wy[2], wy[3], wall.wall_color);
            }
        }
    }

    void Window::ClipBehindCamera(double& x1, double& y1, double& z1, const double& x2, const double& y2, const double& z2)
    {
        float da = y1;  // distance plane a
        float db = y2;  // distance plane b
        float d = da - db;

        Clamp(d, 1, da, false);

        float s = da / d; // intersection factor

        x1 += s * (x2 - (x1));
        y1 += s * (y2 - (y1));
        z1 += s * (z2 - (z1));

        Clamp(y1, 1, y2, false);
    }

    void Window::DrawWall(double x1, double x2, double t1, double t2, double b1, double b2, Color c)
    {
        // differences
        double dyb = b2 - b1;
        double dyt = t2 - t1;
        dyb = RoundToInt(dyb);
        dyt = RoundToInt(dyt);

        double dx = x2 - x1;

        dx = RoundToInt(dx);
        
        if (dx == 0) dx = 1;

        int xs = RoundToInt(x1);

        Clamp(x1, 1, screen_width * resolution, false);
        Clamp(x2, 1, screen_width * resolution, false);

        glColor3ub(c.r, c.g, c.b);
        glBegin(GL_POINTS);

        for (int x = (int)x1; x < (int)x2; x++)
        {
            if (x > 0 && x < screen_width * resolution)
            {
                int y2 = dyb * (x - xs) / dx + b1;
                int y1 = dyt * (x - xs) / dx + t1;

                y1 = RoundToInt(y1, screen_height * resolution / 2);
                y2 = RoundToInt(y2, screen_height * resolution / 2);

                Clamp(y1, 1, screen_height * resolution, false);
                Clamp(y2, 1, screen_height * resolution, false);

                for (int y = y1; y < y2; y++)
                {
                    // Pixel(x, y);
                    glVertex2i(x, y);
                }
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
        static double rs;

        rs += ElapsedTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPointSize(pixel_scale);
        RenderView();

        glfwSwapBuffers(screen);

        glfwPollEvents();
        
        if (!running)
            glfwSetWindowShouldClose(screen, GL_TRUE);
        
        return glfwWindowShouldClose(screen);
    }

    GLFWwindow* Window::GetScreen() const
    {
        return screen;
    }

    void key_callback(GLFWwindow* w, int key, int scancode, int action, int mods)
    {
        for (std::pair<int, Cobra::Object*> o : Cobra::objects)
        {
            o.second->KeyInput(key, action);
        }
    }
}
