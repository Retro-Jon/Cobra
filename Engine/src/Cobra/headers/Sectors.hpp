#pragma once

#include "Core.hpp"
#include <ostream>

namespace Cobra
{
    struct Pos
    {
        double x;
        double y;
        double z;
        double horizontal;
        double vertical;
    };

    enum
    {
        Normal = 0, // External View
        Inverted = 1, // Internal View
        Hollow = 2 // Internal & External View
    };

    struct SectorPoint
    {
        int x;
        int y;
    };

    static std::ostream& operator << (std::ostream& o, SectorPoint& p)
    {
        o << p.x << " " << p.y;
        return o;
    }

    struct Color
    {
        int r, g, b, a;
    };

    struct SectorWall
    {
        SectorPoint p1;
        SectorPoint p2;
        Color wall_color;
        double distance;
    };

    struct Sector
    {
        SectorWall* walls;
        int wall_count;
        int top;
        int bottom;

        Pos position;

        Color top_color;
        Color bottom_color;

        double distance;
        int view;

        int surface;
        int* surf_points;
    };
}