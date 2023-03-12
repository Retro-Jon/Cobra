#include <iostream>

#include "headers/Renderer.hpp"
#include "headers/Window.hpp"
#include "headers/AssetLoader.hpp"

namespace Cobra
{
    Renderer* renderer;

    Renderer::Renderer()
    {
        current_sector = 0;
        view_ports.insert(std::pair<const char*, ViewPort*>("main_view", new ViewPort(0, 0, window->GetWidth(), window->GetHeight())));
    }

    Renderer::~Renderer()
    {
        cameras.clear();
        sectors.clear();
        sector_order.clear();
    }

    // Add n_sector to sectors and push its index to sector_order
    void Renderer::AddSector(Sector n_sector)
    {
        sectors.push_back(n_sector);
        sector_order.push_back(sectors.size() - 1);
    }

    // Clear and replace sectors and sector_order vectors
    void Renderer::SetSectors(std::vector<Sector> n_sectors)
    {
        sectors.clear();
        sector_order.clear();

        for (Sector s : n_sectors)
            AddSector(s);
    }

    std::vector<Sector> Renderer::GetSectors()
    {
        return sectors;
    }

    void Renderer::MoveCamera(const char* name, Pos position)
    {
        cameras[name].position = position;
    }

    // Apply force to camera <name>
    void Renderer::PushCamera(const char* name, Pos force)
    {
        Pos* cc = &cameras[name].position;

        cc->horizontal += force.horizontal * ElapsedTime;
        cc->vertical += force.vertical * ElapsedTime;

        Clamp(cc->horizontal, 0, 360);
        Clamp(cc->vertical, 0, 360);

        double COS = std::cos(cc->horizontal * (M_PI / 180)) * ElapsedTime;
        double SIN = std::sin(cc->horizontal * (M_PI / 180)) * ElapsedTime;
        
        if (force.x != 0)
        {
            if (std::tan(cc->horizontal * (M_PI / 180)) <= 90 || std::tan(cc->horizontal * (M_PI / 180)) >= 270)
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

    // Create new camera <name> at position
    void Renderer::CreateNewCamera(const char* name, Camera n_camera)
    {
        cameras.insert(std::pair<const char*, Camera>(name, n_camera));
        view_ports["main_view"]->BindCamera(name);
        
        std::cout << name << std::endl;
    }

    int Renderer::GetCameraCount()
    {
        return cameras.size();
    }

    // Sort Sectors
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

        // view
        for (int s = 0; s < sector_order.size(); s++)
        {
            for (int w = 0; w < sector_order.size() - limit; w++)
            {
                if (w + 1 < sector_order.size() - limit)
                {
                    if (sectors[sector_order[w + 1]].view == Inverted && sectors[sector_order[w]].view != Inverted)
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

    // Render sectors from perspective of current_camera
    void Renderer::RenderView()
    {
        for (std::pair<std::string, ViewPort*> v : view_ports)
        {
            if (v.second->GetBoundCamera() == "")
                continue;
            
            ViewPort* vp = v.second;
            
            Camera cc = cameras[vp->GetBoundCamera()];

            BubbleSortSectors(cc.position.z);
            
            // Draw Sectors
            for (int index : sector_order)
            {
                Sector cs = sectors[index];
                sectors[index].distance = 0;

                if (cc.position.z < cs.bottom)
                    sectors[index].surface = 1; // Bottom
                else if (cc.position.z > cs.top)
                    sectors[index].surface = 2; // Top
                else
                    sectors[index].surface = 0; // None

                for (int loop = 0; loop < 2; loop++)
                {
                    for (int w = 0; w < cs.wall_count; w++)
                    {
                        // Don't draw wall if distance is greater than max_distance
                        if (loop > 0 && sectors[index].distance > max_distance)
                            continue;
                        
                        SectorWall wall;

                        wall.p1.x = cs.walls[w].p1.x - cc.position.x;
                        wall.p1.y = cs.walls[w].p1.y - cc.position.y;

                        wall.p2.x = cs.walls[w].p2.x - cc.position.x;
                        wall.p2.y = cs.walls[w].p2.y - cc.position.y;

                        wall.wall_color = cs.walls[w].wall_color;

                        double wx[4], wy[4], wz[4];
                        double COS = std::cos(cc.position.horizontal * (M_PI / 180)), SIN = std::sin(cc.position.horizontal * (M_PI / 180));

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
                        wz[0] = cs.bottom - cc.position.z + (((cc.position.vertical - 90) * wy[0]) / 32.00);
                        wz[1] = cs.bottom - cc.position.z + (((cc.position.vertical - 90) * wy[1]) / 32.00);
                        wz[2] = cs.top - cc.position.z + (((cc.position.vertical - 90) * wy[2]) / 32.00);
                        wz[3] = cs.top - cc.position.z + (((cc.position.vertical - 90) * wy[3]) / 32.00);

                        // Screen Positions
                        // X
                        wx[0] = wx[0] * cc.fov / wy[0] + (vp->GetTransformation().w / 2);
                        wx[1] = wx[1] * cc.fov / wy[1] + (vp->GetTransformation().w / 2);

                        // Y
                        wy[0] = wz[0] * cc.fov / wy[0] + (vp->GetTransformation().h / 2);
                        wy[1] = wz[1] * cc.fov / wy[1] + (vp->GetTransformation().h / 2);
                        wy[2] = wz[2] * cc.fov / wy[2] + (vp->GetTransformation().h / 2);
                        wy[3] = wz[3] * cc.fov / wy[3] + (vp->GetTransformation().h / 2);
                        
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
                        
                        DrawWall(wx[0], wx[1], wy[0], wy[1], wy[2], wy[3], sectors[index].surface, cs.surf_points, cs.view, wall.wall_color, cs.top_color, cs.bottom_color, vp);
                    }
                    sectors[index].surface *= -1;
                }
            }
        }
    }

    // Clip sectors in relation to current camera
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

    // Display wall from perspective of current_camera
    void Renderer::DrawWall(double x1, double x2, double t1, double t2, double b1, double b2, int surface, int* points, int view, Color wc, Color tc, Color bc, ViewPort* vp)
    {
        // differences
        int dyb = floor(b2) - floor(b1);
        int dyt = floor(t2) - floor(t1);

        int dx = floor(x2) - floor(x1);

        if (dx == 0) dx = 1;

        glBegin(GL_POINTS);
        
        int xs = x1, xf = x2;

        if (x1 < 0) x1 = 0;
        if (x2 < 0) x2 = 0;
        if (x1 > vp->GetTransformation().w) x1 = vp->GetTransformation().w;
        if (x2 > vp->GetTransformation().w) x2 = vp->GetTransformation().w;

        for (int x = xs; x < xf; x++)
        {
            if (x >= 0 && x < vp->GetTransformation().w)
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
                    if (surface == -1 && bc.a == 1)          // Bottom
                    {
                        glColor3ub(bc.r, bc.g, bc.b);
                        for (int y = points[x]; y < y1; y++)
                            glVertex2i(x, y);
                    } else if (surface == -2 && tc.a == 1)   // Top
                    {
                        glColor3ub(tc.r, tc.g, tc.b);
                        for (int y = y2; y < points[x]; y++)
                            glVertex2i(x, y);
                    }
                }

                if (wc.a == 1)
                {
                    // Normal Wall
                    glColor3ub(wc.r, wc.g, wc.b);

                    Clamp(y1, 0, vp->GetTransformation().h, false);
                    Clamp(y2, 0, vp->GetTransformation().h, false);

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
        }
        glEnd();
    }

    // Draw a GL_POINT at position x,y with color r,g,b
    void Renderer::Pixel(int x, int y, int r, int g, int b)
    {
        glColor3ub(r, g, b);
        glBegin(GL_POINTS);
        glVertex2i(x, y);
        glEnd();
    }
}