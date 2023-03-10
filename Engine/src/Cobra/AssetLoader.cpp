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
            SectorPoint last_point;
            bool last_point_set = false;
            Color c_color;

            Sector current_sector;
            
            while (std::getline(file, line))
            {
                std::stringstream ss(line);
                std::string directive;
                if (line[0] == '#')
                    continue;

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
                        last_point_set = false;
                        res.push_back(current_sector);
                        break;
                    }

                    if (directive == "close")
                    {
                        current_sector.walls[current_wall].p1 = last_point;
                        current_sector.walls[current_wall].p2 = current_sector.walls[0].p1;
                        current_wall++;
                        break;
                    }

                    if (directive == "gap")
                    {
                        last_point_set = false;
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
                                    c_color.r = std::stoi(line);
                                    break;
                                case 1:
                                    c_color.g = std::stoi(line);
                                    break;
                                case 2:
                                    c_color.b = std::stoi(line);
                                    break;
                                case 3:
                                    c_color.a = std::stoi(line);
                                    break;
                            }
                        } else if (directive == "p")
                        {
                            if (last_point_set)
                            {
                                current_sector.walls[current_wall].p1 = last_point;

                                switch (loop_num)
                                {
                                    case 0:
                                        current_sector.walls[current_wall].p2.x = std::stoi(line) + current_sector.position.x;
                                        break;
                                    case 1:
                                        current_sector.walls[current_wall].p2.y = std::stoi(line) + current_sector.position.y;
                                        last_point = current_sector.walls[current_wall].p2;
                                        current_sector.walls[current_wall].wall_color = c_color;
                                        current_wall++;
                                        break;
                                }
                            } else {
                                switch (loop_num)
                                {
                                    case 0:
                                        last_point.x = std::stoi(line) + current_sector.position.x;
                                        break;
                                    case 1:
                                        last_point.y = std::stoi(line) + current_sector.position.y;
                                        last_point_set = true;
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

    void AssetLoader::SaveSectors(std::string path, std::vector<Sector> sectors)
    {
        if (path == "")
            ERROR("LoadSector: No path specified.");
        
        std::ofstream file;
        file.open("Sectors/" + path);

        for (Sector s : sectors)
        {
            file << "start\n";
            file << "c " << s.wall_count << "\n";
            file << "v ";

            if (s.view == Normal)
                file << "Normal\n";
            else if (s.view == Inverted)
                file << "Inverted\n";
            else
                file << "Hollow\n";
            
            file << "position ";
            file << s.position.x << "," << s.position.y << "," << s.position.z << "\n";

            file << "t " << s.top << "\n";
            file << "b " << s.bottom << "\n";

            file << "tc ";
            file << s.top_color.r << "," << s.top_color.g << "," << s.top_color.b << "," << s.top_color.a << "\n";

            file << "bc ";
            file << s.bottom_color.r << "," << s.bottom_color.g << "," << s.bottom_color.b << "," << s.bottom_color.a << "\n\n";

            Color last_color = s.walls[0].wall_color;
            SectorPoint last_point = s.walls[0].p1;

            file << "wc ";
            file << last_color.r << "," << last_color.g << "," << last_color.b << "," << last_color.a << "\n";
            file << "p " << last_point.x << "," << last_point.y << "\n";

            for (int i = 0; i < s.wall_count; i++)
            {
                SectorWall sw = s.walls[i];

                if (sw.wall_color != last_color)
                {
                    last_color = sw.wall_color;
                    file << "wc ";
                    file << last_color.r << "," << last_color.g << "," << last_color.b << "," << last_color.a << "\n";
                }

                if (sw.p1 != last_point)
                {
                    file << "p " << sw.p1.x << "," << sw.p1.y << "\n";
                }

                file << "p " << sw.p2.x << "," << sw.p2.y << "\n";
                last_point = sw.p2;
            }

            file << "\nbreak\n\n";
        }
    }
}
