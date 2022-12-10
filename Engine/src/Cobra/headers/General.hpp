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
            float v = ((float)i) * (M_PI / 180);
            M.sin[i] = std::sin(v);
            M.cos[i] = std::cos(v);
            M.tan[i] = std::tan(v);
        }
    }

    template <typename T> static void Clamp(T& value, const T& lower, const T& upper)
    {
        T difference = upper - lower;

        if (value > upper - 1)
            value -= difference;
        
        if (value < lower)
            value += difference;
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