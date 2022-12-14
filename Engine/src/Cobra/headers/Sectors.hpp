#pragma once

#include "Core.hpp"

namespace Cobra
{
    struct SectorPoint
    {
        int x;
        int y;
    };

    struct SectorWall
    {
        SectorPoint p1;
        SectorPoint p2;
    };

    struct Sector
    {
        SectorWall* walls;
        int wall_count;
        int top;
        int bottom;
    };
}