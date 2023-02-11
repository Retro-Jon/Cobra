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

    std::vector<Sector> AssetLoader::LoadSectors(std::string path, int amount_surf_points)
    {
        if (path == "")
            ERROR("LoadSector: No path specified.");
        
        if (amount_surf_points < 1)
            ERROR("LoadSector: amount_surf_points cannot be less than 1.");
        
        std::vector<Sector> res;

        std::ifstream file;
        file.open("Sectors/" + path);
        std::string line;

        if (file.is_open())
        {
            int current_wall = 0;
            bool first_point = true;

            Sector current_sector;
            
            while (std::getline(file, line))
            {
                std::stringstream ss(line);
                std::string directive;

                while (std::getline(ss, directive, ' '))
                {
                    if (directive == "start")
                    {
                        current_sector.distance = 0;
                        current_sector.surf_points = new int[amount_surf_points];
                        current_wall = 0;
                    }

                    if (directive == "break")
                    {
                        res.push_back(current_sector);
                        break;
                    }
                    int loop_num = 0;

                    while (std::getline(ss, line, ','))
                    {
                        if (line.empty())
                            continue;
                        
                        if (directive == "c")
                        {
                            current_sector.wall_count = std::stoi(line);
                            current_sector.walls = new SectorWall[current_sector.wall_count];
                        }
                        else if (directive == "v")
                        {
                            if (line == "Normal")
                                current_sector.view = Normal;
                            else if (line == "Hollow")
                                current_sector.view = Hollow;
                            else if (line == "Inverted")
                                current_sector.view = Inverted;
                        } else if (directive == "t")
                            current_sector.top = std::stoi(line) + current_sector.position.z;
                        else if (directive == "b")
                            current_sector.bottom = std::stoi(line) + current_sector.position.z;
                        else if (directive == "tc")
                        {
                            switch (loop_num)
                            {
                                case 0:
                                    current_sector.top_color.r = std::stoi(line);
                                    break;
                                case 1:
                                    current_sector.top_color.g = std::stoi(line);
                                    break;
                                case 2:
                                    current_sector.top_color.b = std::stoi(line);
                                    break;
                                case 3:
                                    current_sector.top_color.a = std::stoi(line);
                                    break;
                            }
                        } else if (directive == "bc")
                        {
                            switch (loop_num)
                            {
                                case 0:
                                    current_sector.bottom_color.r = std::stoi(line);
                                    break;
                                case 1:
                                    current_sector.bottom_color.g = std::stoi(line);
                                    break;
                                case 2:
                                    current_sector.bottom_color.b = std::stoi(line);
                                    break;
                                case 3:
                                    current_sector.bottom_color.a = std::stoi(line);
                                    break;
                            }
                        } else if (directive == "wc")
                        {
                            switch (loop_num)
                            {
                                case 0:
                                    current_sector.walls[current_wall].wall_color.r = std::stoi(line);
                                    break;
                                case 1:
                                    current_sector.walls[current_wall].wall_color.g = std::stoi(line);
                                    break;
                                case 2:
                                    current_sector.walls[current_wall].wall_color.b = std::stoi(line);
                                    break;
                                case 3:
                                    current_sector.walls[current_wall].wall_color.a = std::stoi(line);
                                    break;
                            }
                        } else if (directive == "p")
                        {
                            if (first_point)
                            {
                                switch (loop_num)
                                {
                                    case 0:
                                        current_sector.walls[current_wall].p1.x = std::stoi(line) + current_sector.position.x;
                                        break;
                                    case 1:
                                        current_sector.walls[current_wall].p1.y = std::stoi(line) + current_sector.position.y;
                                        first_point = !first_point;
                                        break;
                                }
                            } else {
                                switch (loop_num)
                                {
                                    case 0:
                                        current_sector.walls[current_wall].p2.x = std::stoi(line) + current_sector.position.x;
                                        break;
                                    case 1:
                                        current_sector.walls[current_wall].p2.y = std::stoi(line) + current_sector.position.y;
                                        first_point = !first_point;
                                        current_wall++;
                                        break;
                                }
                            }
                        } else if (directive == "position")
                        {
                            switch (loop_num)
                            {
                                case 0:
                                    current_sector.position.x = std::stoi(line);
                                    break;
                                case 1:
                                    current_sector.position.y = std::stoi(line);
                                    break;
                                case 2:
                                    current_sector.position.z = std::stoi(line);
                                    break;
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
