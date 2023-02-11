#include <iostream>

#include "headers/Renderer.hpp"
#include "headers/Window.hpp"
#include "headers/AssetLoader.hpp"

namespace Cobra
{
    Renderer* renderer;

    Renderer::Renderer(int Fov)
    {
        current_camera = "";
        current_sector = 0;
        fov = Fov;
    }

    Renderer::~Renderer()
    {
        cameras.clear();
        sectors.clear();
        sector_order.clear();
        renderer = nullptr;
    }

    void Renderer::AddSector(Sector n_sector)
    {
        sectors.push_back(n_sector);
        sector_order.push_back(sectors.size() - 1);
    }

    bool Renderer::SwitchActiveCamera(int camera)
    {
        if (camera < cameras.size())
        {
            current_camera = camera;
            return true;
        }

        return false;
    }

    void Renderer::MoveActiveCamera(Pos force)
    {
        MoveCamera(current_camera, force);
    }

    void Renderer::MoveCamera(std::string name, Pos force)
    {
        cameras[name] = force;
    }

    void Renderer::PushCamera(std::string name, Pos force)
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

    void Renderer::CreateNewCamera(std::string name, Pos position)
    {
        cameras.insert(std::pair<std::string, Pos>(name, position));
        
        if (current_camera == "")
            current_camera = name;
        
        std::cout << name << std::endl;
    }

    int Renderer::GetCameraCount()
    {
        return cameras.size();
    }

    void Renderer::BubbleSortSectors(double z)
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

    void Renderer::RenderView()
    {
        if (fov == 0)
            return;

        const double multiplier = window->GetWidth() / fov;
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
                        wx[0] = wx[0] * fov / wy[0] + (window->GetWidth() * window->GetResolution() / 2);
                        wx[1] = wx[1] * fov / wy[1] + (window->GetWidth() * window->GetResolution() / 2);

                        // Y
                        wy[0] = wz[0] * fov / wy[0] + (window->GetHeight() * window->GetResolution() / 2);
                        wy[1] = wz[1] * fov / wy[1] + (window->GetHeight() * window->GetResolution() / 2);
                        wy[2] = wz[2] * fov / wy[2] + (window->GetHeight() * window->GetResolution() / 2);
                        wy[3] = wz[3] * fov / wy[3] + (window->GetHeight() * window->GetResolution() / 2);
                        
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

    void Renderer::ClipBehindCamera(double& x1, double& y1, double& z1, const double& x2, const double& y2, const double& z2)
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

    void Renderer::DrawWall(double x1, double x2, double t1, double t2, double b1, double b2, int surface, int* points, int view, Color wc, Color tc, Color bc)
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
        if (x1 > window->GetWidth()) x1 = window->GetWidth();
        if (x2 > window->GetWidth()) x2 = window->GetWidth();

        for (int x = xs; x < xf; x++)
        {
            if (x >= 0 && x < window->GetWidth())
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

                Clamp(y1, 0, window->GetHeight(), false);
                Clamp(y2, 0, window->GetHeight(), false);

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

    void Renderer::Pixel(int x, int y, int r, int g, int b)
    {
        glColor3ub(r, g, b);
        glBegin(GL_POINTS);
        glVertex2i(x, y);
        glEnd();
    }
}