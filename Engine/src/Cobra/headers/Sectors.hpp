#pragma once

#include "Core.hpp"

namespace Cobra
{
    struct SectorPoint
    {
        int x;
        int y;
    };

    struct Sector
    {
        SectorPoint* points;
        int point_count;
        int top;
        int bottom;
    };
}