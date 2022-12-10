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
        s.points[2] = (SectorPoint){.x = 4, .y = 2};
        s.points[3] = (SectorPoint){.x = -4, .y = 2};

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
        MoveCamera(current_camera, force);
    }

    void Window::MoveCamera(std::string name, Pos force)
    {
        cameras[name] += force;
    }

    void Window::PushCamera(std::string name, Pos force)
    {
        Pos* cc = &cameras[name];

        cc->angle += force.angle;

        Clamp(cc->angle, 0, 360);
        
        if (force.x != 0)
        {
            if (M.tan[cc->angle] <= 90 || M.tan[cc->angle] >= 270)
            {
                cc->x += force.x * M.cos[cc->angle];
                cc->y -= force.x * M.sin[cc->angle];
            } else {
                cc->x -= force.x * M.cos[cc->angle];
                cc->y += force.x * M.sin[cc->angle];
            }
        }
        if (force.y != 0)
        {
            cc->x += force.x * M.cos[cc->angle];
            cc->y += force.y * M.sin[cc->angle];
        }

        cc->z += force.z;
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
        const int dist = 10;
        const float fov = 45;

        Pos cc = cameras[current_camera];
        // std::cout << cc << std::endl;

        float offset_x = M.sin[cc.angle];
        float offset_y = M.cos[cc.angle];

        for (int w = 0; w < sectors[current_sector].wall_count; w++)
        {
            Sector s = sectors[current_sector];
            SectorWall sw = s.walls[w];

            auto CastRay = [cc, sw, dist, offset_x, offset_y]()
            {
                float rx = 0;
                float ry = 0;
                
                float m = (sw.y1 - sw.y2) / (sw.x1 - sw.x2);

                for (float d = 1; d < dist; d += 0.1)
                {
                    rx += offset_x;
                    ry += offset_y;

                    if ((rx >= sw.x1 || rx >= sw.x2) && (ry >= sw.y1 || ry >= sw.y2))
                        return d;
                }

                return (float)dist;
            };

            bool cc_north = (cc.y < sw.y1 || cc.y < sw.y2);
            bool cc_south = (cc.y > sw.y1 || cc.y > sw.y2);
            bool cc_east = (cc.x > sw.x1 || cc.x > sw.x2);
            bool cc_west = (cc.x < sw.x1 || cc.x < sw.x2);

            if ((sw.facesNorth && cc_north && !cc_south) || (sw.facesSouth && cc_south && !cc_north) || (sw.facesEast && cc_east && !cc_west) || (sw.facesWest && cc_west && !cc_east))
            {
                glColor3ub(255, 255, 255);
                glBegin(GL_LINES);
                for (int screen_x = -screen_width / 2; screen_x < screen_width / 2; screen_x += 1)
                {
                    int a = (cc.angle + (screen_x));
                    float d = CastRay();

                    if (d < dist)
                    {
                        glVertex2f(screen_x, d);
                        glVertex2f(screen_x, -d);
                    }
                }
                glEnd();
            }

            glBegin(GL_POINTS);
            glColor3ub(255, 0, 0);
            glVertex2f(cc.x, cc.y);
            glColor3ub(0, 0, 255);
            glVertex2f(cc.x + M.sin[cc.angle] * 6, cc.y + M.cos[cc.angle] * 6);
            glEnd();
        }

        tick++;
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
