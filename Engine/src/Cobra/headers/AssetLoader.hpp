#pragma once

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

            Sector LoadSector(std::string path = "");
    };

    COBRA_API extern AssetLoader* assetloader;
}