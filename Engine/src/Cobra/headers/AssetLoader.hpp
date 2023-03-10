#pragma once

#include <vector>

#include "Core.hpp"
#include "Sectors.hpp"
#include "General.hpp"

namespace Cobra
{
    class COBRA_API AssetLoader
    {
        private:
        public:
            AssetLoader();
            ~AssetLoader();

            std::vector<Sector> LoadSectors(std::string path = "", int amount_surf_points = 0);
            void SaveSectors(std::string path, std::vector<Sector> sectors);
    };

    COBRA_API extern AssetLoader* assetloader;
}