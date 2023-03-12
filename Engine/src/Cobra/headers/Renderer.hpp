#pragma once

#include <vector>
#include <map>
#include <string>

#include "Core.hpp"
#include "Sectors.hpp"
#include "General.hpp"
#include "ViewPort.hpp"

namespace Cobra
{
    struct Camera
    {
        Pos position;
        int fov;
    };

    class COBRA_API Renderer
    {
        private:
            std::map<const char*, Camera> cameras;
            std::map<const char*, ViewPort*> view_ports;
            std::vector<Sector> sectors;
            std::vector<int> sector_order;
            int current_sector;
            int max_distance = 200;
            ViewPort* main_view;
        
        public:
            Renderer();
            ~Renderer();

            void AddSector(Sector n_sector);
            void SetSectors(std::vector<Sector> n_sectors);
            std::vector<Sector> GetSectors();
            void MoveCamera(const char* name = "", Pos position = (Pos){.x = 0, .y = 0, .z = 0, .horizontal = 0, .vertical = 0});
            void PushCamera(const char* name = "", Pos force = (Pos){.x = 0, .y = 0, .z = 0, .horizontal = 0, .vertical = 0});
            void CreateNewCamera(const char* name, Camera n_camera = (Camera){.position = (Pos){.x = 0, .y = 0, .z = 0, .horizontal = 0, .vertical = 0}, .fov = 0});
            int GetCameraCount();

            void BubbleSortSectors(double z);
            void RenderView();

            void ClipBehindCamera(double& x1, double& y1, double& z1, const double& x2, const double& y2, const double& z2);
            void DrawWall(double x1, double x2, double t1, double t2, double b1, double b2, int surface, int* points, int view, Color wc, Color tc, Color bc, ViewPort* vp);
            void Pixel(int x, int y, int r = 255, int g = 255, int b = 255);
    };

    COBRA_API extern Renderer* renderer;
}