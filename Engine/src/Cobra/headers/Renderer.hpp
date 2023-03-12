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
    class COBRA_API Renderer
    {
        private:
            int fov;
            std::map<std::string, Pos> cameras;
            std::string current_camera;
            std::vector<Sector> sectors;
            std::vector<int> sector_order;
            int current_sector;
            int max_distance = 200;
            ViewPort* main_view;
        
        public:
            Renderer(int Fov = 200);
            ~Renderer();

            void AddSector(Sector n_sector);
            void SetSectors(std::vector<Sector> n_sectors);
            std::vector<Sector> GetSectors();
            bool SwitchActiveCamera(int camera = 0);
            void MoveActiveCamera(Pos force = (Pos){.x = 0, .y = 0, .z = 0, .horizontal = 0, .vertical = 0});
            void MoveCamera(std::string name = "", Pos position = (Pos){.x = 0, .y = 0, .z = 0, .horizontal = 0, .vertical = 0});
            void PushCamera(std::string name = "", Pos force = (Pos){.x = 0, .y = 0, .z = 0, .horizontal = 0, .vertical = 0});
            void CreateNewCamera(std::string name, Pos position = (Pos){.x = 0, .y = 0, .z = 0, .horizontal = 0, .vertical = 0});
            int GetCameraCount();

            void BubbleSortSectors(double z);
            void RenderView();

            void ClipBehindCamera(double& x1, double& y1, double& z1, const double& x2, const double& y2, const double& z2);
            void DrawWall(double x1, double x2, double t1, double t2, double b1, double b2, int surface, int* points, int view, Color wc, Color tc, Color bc);
            void Pixel(int x, int y, int r = 255, int g = 255, int b = 255);
    };

    COBRA_API extern Renderer* renderer;
}