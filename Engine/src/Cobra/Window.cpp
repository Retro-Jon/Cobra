#include "headers/Window.hpp"
#include "headers/Object.hpp"

#include <algorithm>

namespace Cobra
{
    Window* window;

    Window::Window(int Width, int Height, int Resolution, int Pixel_Scale, const char* Title)
    {
        running = true;
        window = this;
        
        resolution = Resolution;
        screen_width = Width;
        screen_height = Height;
        pixel_scale = Pixel_Scale / resolution;

        title = Title;

        current_camera = "";
        current_sector = 0;

        Sector s;

        s.wall_count = 4;
        s.walls = new SectorWall[s.wall_count];

        s.points = new SectorPoint[s.wall_count];
        s.points[0] = (SectorPoint){.x = -2, .y = -2};
        s.points[1] = (SectorPoint){.x = 2, .y = -2};
        s.points[2] = (SectorPoint){.x = 2, .y = 2};
        s.points[3] = (SectorPoint){.x = -2, .y = 2};

        s.top = 1;
        s.bottom = 0;

        for (int w = 0; w < s.wall_count; w++)
        {
            s.walls[w].x1 = s.points[w].x;
            s.walls[w].y1 = s.points[w].y;

            if (w + 1 < s.wall_count)
            {
                s.walls[w].x2 = s.points[w + 1].x;
                s.walls[w].y2 = s.points[w + 1].y;
            } else {
                s.walls[w].x2 = s.points[0].x;
                s.walls[w].y2 = s.points[0].y;
            }
        }

        s.walls[0].facesNorth = true;

        s.walls[1].facesSouth = true;
        s.walls[1].facesEast = true;
        
        s.walls[2].facesSouth = true;

        s.walls[3].facesSouth = true;
        s.walls[3].facesWest = true;

        sectors.push_back(s);

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
        glOrtho(-(screen_width / 2) * resolution, (screen_width / 2) * resolution, -(screen_height / 2) * resolution, (screen_height / 2) * resolution, -1.0f, 1.0f);
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
        cameras[current_camera] += force;
    }

    void Window::MoveCamera(std::string name, Pos force)
    {
        cameras[name] += force;
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
        static int tick = 0;
        const float fov = 45;

        Pos cc = cameras[current_camera];

        float CS = M.cos[cc.angle];
        float SN = M.sin[cc.angle];
        
        for (int w = 0; w < sectors[current_sector].wall_count; w++)
        {
            Sector s = sectors[current_sector];
            SectorWall sw = s.walls[w];

            bool cc_north = (cc.y < sw.y1 || cc.y < sw.y2);
            bool cc_south = (cc.y > sw.y1 || cc.y > sw.y2);
            bool cc_east = (cc.x > sw.x1 || cc.x > sw.x2);
            bool cc_west = (cc.x < sw.x1 || cc.x < sw.x2);

            if ((sw.facesNorth && cc_north && !cc_south) || (sw.facesSouth && cc_south && !cc_north) || (sw.facesEast && cc_east && !cc_west) || (sw.facesWest && cc_west && !cc_east))
            {
                // screen coordinates
                float py[4] = {0};
                float px[4] = {0};

                // // left side
                // float m1;
                // float d1;
                // float x1;
                // float h1;
                // float a1;

                // // right side
                // float m2;
                // float d2;
                // float x2;
                // float h2;
                // float a2;
                
                // // slope
                // m1 = (cc.y - sw.y1) / (cc.x - sw.x1);
                // m2 = (cc.y - sw.y2) / (cc.x - sw.x2);

                // // x intercepts
                // x1 = -((sw.y1 - m1 * sw.x1) / m1) * 2;
                // x2 = -((sw.y2 - m2 * sw.x2) / m2) * 2;

                // // angles
                // a1 = atan2(x1, 10);
                // a2 = atan2(x2, 10);

                // // distance to wall sides
                // d1 = sqrt(pow((sw.x1 - cc.x), 2) + pow((sw.y1 - cc.y), 2)) * M.cos[cc.angle];
                // d2 = sqrt(pow((sw.x2 - cc.x), 2) + pow((sw.y2 - cc.y), 2)) * M.cos[cc.angle];

                // // wall side height
                // h1 = (abs(s.bottom) + abs(s.top)) / d1 * 80;
                // h2 = (abs(s.bottom) + abs(s.top)) / d2 * 80;

                // px[0] = x1;
                // px[1] = x2;
                // px[2] = x2;
                // px[3] = x1;

                // py[0] = h1;
                // py[1] = h2;
                // py[2] = -h2;
                // py[3] = -h1;
                
                glColor3ub(100, 100, 100);

                glBegin(GL_QUADS);
                glVertex2f(px[0], py[0]);
                glVertex2f(px[1], py[1]);
                glVertex2f(px[2], py[2]);
                glVertex2f(px[3], py[3]);
                glEnd();

                Pixel(px[0], py[0]);
                Pixel(px[1], py[1]);
                Pixel(px[2], py[2]);
                Pixel(px[3], py[3]);
            }
        }

        tick++;
    }

    void Window::Pixel(float x, float y, int r, int g, int b)
    {
        glColor3ub(r, g, b);
        glBegin(GL_POINTS);
        glVertex2i(x, y);
        glEnd();
    }

    bool Window::Display()
    {
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
