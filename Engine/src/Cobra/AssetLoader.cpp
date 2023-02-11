#include "headers/AssetLoader.hpp"
#include <fstream>
#include <sstream>

namespace Cobra
{
    AssetLoader* assetloader;
    AssetLoader::AssetLoader(){}

    AssetLoader::~AssetLoader()
    {
        assetloader = nullptr;
    }

    Sector AssetLoader::LoadSector(std::string path, int amount_surf_points)
    {
        if (path == "")
            ERROR("LoadSector: No path specified.");
        
        if (amount_surf_points < 1)
            ERROR("LoadSector: amount_surf_points cannot be less than 1.");
        
        Sector res;
        
        res.distance = 0;
        res.surf_points = new int[amount_surf_points];

        std::ifstream file;
        file.open("Sectors/" + path);
        std::string line;

        if (file.is_open())
        {
            int current_wall = 0;
            bool first_point = true;
            
            while (std::getline(file, line))
            {
                std::stringstream ss(line);
                std::string directive;

                while (std::getline(ss, directive, ' '))
                {
                    int loop_num = 0;

                    while (std::getline(ss, line, ','))
                    {
                        if (line.empty())
                            continue;

                        if (directive == "c")
                        {
                            res.wall_count = std::stoi(line);
                            res.walls = new SectorWall[res.wall_count];
                        }
                        else if (directive == "v")
                        {
                            if (line == "Normal")
                                res.view = Normal;
                            else if (line == "Hollow")
                                res.view = Hollow;
                            else if (line == "Inverted")
                                res.view = Inverted;
                        } else if (directive == "t")
                            res.top = std::stoi(line);
                        else if (directive == "b")
                            res.bottom = std::stoi(line);
                        else if (directive == "tc")
                        {
                            switch (loop_num)
                            {
                                case 0:
                                    res.top_color.r = std::stoi(line);
                                    break;
                                case 1:
                                    res.top_color.g = std::stoi(line);
                                    break;
                                case 2:
                                    res.top_color.b = std::stoi(line);
                                    break;
                            }
                        } else if (directive == "bc")
                        {
                            switch (loop_num)
                            {
                                case 0:
                                    res.bottom_color.r = std::stoi(line);
                                    break;
                                case 1:
                                    res.bottom_color.g = std::stoi(line);
                                    break;
                                case 2:
                                    res.bottom_color.b = std::stoi(line);
                                    break;
                            }
                        } else if (directive == "wc")
                        {
                            switch (loop_num)
                            {
                                case 0:
                                    res.walls[current_wall].wall_color.r = std::stoi(line);
                                    break;
                                case 1:
                                    res.walls[current_wall].wall_color.g = std::stoi(line);
                                    break;
                                case 2:
                                    res.walls[current_wall].wall_color.b = std::stoi(line);
                                    break;
                            }
                        } else if (directive == "p")
                        {
                            if (first_point)
                            {
                                switch (loop_num)
                                {
                                    case 0:
                                        res.walls[current_wall].p1.x = std::stoi(line);
                                        break;
                                    case 1:
                                        res.walls[current_wall].p1.y = std::stoi(line);
                                        first_point = !first_point;
                                        break;
                                }
                            } else {
                                switch (loop_num)
                                {
                                    case 0:
                                        res.walls[current_wall].p2.x = std::stoi(line);
                                        break;
                                    case 1:
                                        res.walls[current_wall].p2.y = std::stoi(line);
                                        first_point = !first_point;
                                        current_wall++;
                                        break;
                                }
                            }
                        }

                        loop_num++;
                    }
                }
            }
            file.close();
        }

        return res;
    }
}
