#pragma once

#include "Core.hpp"

namespace Cobra
{
    struct SectorPoint
    {
        int x;
        int y;
        bool FacesNorth;
        bool FacesEast;
    };

    struct Sector
    {
        SectorPoint* points;
        int point_count;
        int top;
        int bottom;
    };
}