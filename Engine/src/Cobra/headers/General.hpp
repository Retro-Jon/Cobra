#pragma once

#include "Core.hpp"
#define _USE_MATH_DEFINES
#include <cmath>
#include <ostream>

namespace Cobra
{
    struct Math
    {
        float sin[360];
        float cos[360];
        float tan[360];
    };

    COBRA_API extern struct Math M;

    static void PopulateTrig()
    {
        for (int i = 0; i < 360; i++)
        {
            M.sin[i] = std::sin(i);
            M.cos[i] = std::cos(i);
            M.tan[i] = std::tan(i);
        }
    }
    
    struct Pos
    {
        float x;
        float y;
        float z;
        int angle;
    };

    Pos operator + (const Pos& a, const Pos& b);
    Pos operator - (const Pos& a, const Pos& b);
    Pos& operator += (Pos& a, const Pos& b);
    Pos& operator -= (Pos& a, const Pos& b);
    std::ostream& operator << (std::ostream& o, const Pos& p);
}