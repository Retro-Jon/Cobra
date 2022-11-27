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
        int x1, y1;
        int x2, y2;
        int portal = -1;
        bool facesNorth = false;
        bool facesSouth = false;
        bool facesEast = false;
        bool facesWest = false;
    };

    struct Sector
    {
        SectorPoint* points;
        int wall_count;
        SectorWall* walls;
        int top;
        int bottom;
    };
}