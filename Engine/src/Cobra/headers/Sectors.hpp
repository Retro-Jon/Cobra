#pragma once

#include "Core.hpp"

namespace Cobra
{
    struct SectorPoint
    {
        int x;
        int y;
    };

    struct Color
    {
        int r, g, b;
    };

    struct SectorWall
    {
        SectorPoint p1;
        SectorPoint p2;
        Color wall_color;
    };

    struct Sector
    {
        SectorWall* walls;
        int wall_count;
        int top;
        int bottom;

        Color top_color;
        Color bottom_color;
    };
}