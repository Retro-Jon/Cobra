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
        s.point_count = 4;
        s.points = new SectorPoint[s.point_count];

        s.points[0] = (SectorPoint){.x = -10, .y = -10};
        s.points[1] = (SectorPoint){.x = 10, .y = -10};
        s.points[2] = (SectorPoint){.x = 10, .y = 10};
        s.points[3] = (SectorPoint){.x = -10, .y = 10};

        s.top = 1;
        s.bottom = 0;

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

        double COS = M.cos[(int)cc->angle];
        double SIN = M.sin[(int)cc->angle];
        
        if (force.x != 0)
        {
            if (M.tan[(int)cc->angle] <= 90 || M.tan[(int)cc->angle] >= 270)
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
        const double fov = 30;
        const double multiplier = screen_width / fov;
        const double view_distance = 50;
        const int view_bound = 50;

        Pos cc = cameras[current_camera];

        glBegin(GL_QUADS);

        double m1, d1, t1, b1;
        double m2, d2, t2, b2;

        std::pair<double, double> intercept1;
        std::pair<double, double> intercept2;
        
        SectorPoint points[4];

        std::pair<double, double> last_intercept = std::pair<double, double>(0, 0);
        double last_d = 1000;
        
        for (int cs = current_sector; cs < sectors.size(); cs++)
        {
            for (int pc = 0; pc < sectors[cs].point_count; pc++)
            {
                SectorPoint p1 = sectors[cs].points[pc];
                points[pc] = p1;
                SectorPoint p2;
                
                if (pc + 1 < sectors[cs].point_count)
                    p2 = sectors[cs].points[pc + 1];
                else
                    p2 = sectors[cs].points[0];
                
                double s1, s2;

                {
                    d1 = std::sqrt(pow((p1.x - (cc.x * M.tan[(int)cc.angle])), 2) + pow((p1.y - cc.y), 2));

                    d2 = std::sqrt(pow((p2.x - (cc.x * M.tan[(int)cc.angle])), 2) + pow((p2.y - cc.y), 2));

                    intercept1 = LineGraph(cc.x + M.cos[(int)cc.angle], cc.y - M.sin[(int)cc.angle], cc.x, cc.y, p1.x, p1.y);
                    intercept2 = LineGraph(cc.x + M.cos[(int)cc.angle], cc.y - M.sin[(int)cc.angle], cc.x, cc.y, p2.x, p2.y);

                    intercept1.first -= (cc.x * M.cos[(int)cc.angle]) - (cc.y * M.sin[(int)cc.angle]);
                    intercept1.second += (cc.x * M.cos[(int)cc.angle]) + (cc.y * M.sin[(int)cc.angle]);
                    
                    intercept2.first -= (cc.x * M.cos[(int)cc.angle]) - (cc.y * M.sin[(int)cc.angle]);
                    intercept2.second += (cc.x * M.cos[(int)cc.angle]) + (cc.y * M.sin[(int)cc.angle]);

                    Clamp(intercept1.first, -fov, fov, false);
                    Clamp(intercept1.second, -fov, fov, false);
                    Clamp(intercept2.first, -fov, fov, false);
                    Clamp(intercept2.second, -fov, fov, false);

                    t1 = (screen_height / 2 - d1) - sectors[cs].top;
                    t2 = (screen_height / 2 - d2) - sectors[cs].top;

                    Clamp(t1, 0, (screen_height / 2), false);
                    Clamp(t2, 0, (screen_height / 2), false);

                    b1 = (-screen_height / 2 + d1) - sectors[cs].bottom;
                    b2 = (-screen_height / 2 + d2) - sectors[cs].bottom;

                    Clamp(b1, -screen_height / 2, 0, false);
                    Clamp(b2, -screen_height / 2, 0, false);
                }

                double adjustment;

                s1 = intercept1.first * multiplier;
                s2 = intercept2.first * multiplier;
                adjustment = M.cos[(int)cc.angle];
                
                glColor3ub(0, 0, 255);

                switch (pc)
                {
                    case 0:
                        glColor3ub(255, 0, 0);
                        break;
                    case 1:
                        glColor3ub(0, 0, 255);
                        break;
                    case 2:
                        glColor3ub(255, 255, 0);
                        break;
                    case 3:
                        glColor3ub(0, 255, 0);
                        break;
                }

                glVertex2f(s1, t1 * adjustment);
                glVertex2f(s1, b1 * adjustment);
                glVertex2f(s2, b2 * adjustment);
                glVertex2f(s2, t2 * adjustment);
            }
            
            glEnd();

            UsedScreenSpace.clear();

            glColor3ub(255, 255, 255);
            glBegin(GL_POINTS);

            for (int i = 0; i < sectors[cs].point_count; i++)
                glVertex2f(points[i].x, points[i].y);
            
            glVertex2f(cc.x, cc.y);
            
            double a = cc.angle + 90;
            Clamp(a, 0, 360);

            glVertex2f(cc.x - M.cos[(int)a] * 6, cc.y + M.sin[(int)a] * 6);
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
