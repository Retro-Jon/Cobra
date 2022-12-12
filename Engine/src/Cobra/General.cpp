#include "headers/General.hpp"
#include <iostream>

namespace Cobra
{
    Math M;

    Pos operator + (const Pos& a, const Pos& b)
    {
        Pos res;
        res.x = a.x + b.x;
        res.y = a.y + b.y;
        res.z = a.z + b.z;
        res.angle = a.angle + b.angle;

        Clamp(res.angle, 0, 360);
        
        return res;
    }

    Pos operator - (const Pos& a, const Pos& b)
    {
        Pos res;
        res.x = a.x - b.x;
        res.y = a.y - b.y;
        res.z = a.z - b.z;
        res.angle = a.angle - b.angle;

        Clamp(res.angle, 0, 360);
        
        return res;
    }

    Pos& operator += (Pos& a, const Pos& b)
    {
        a = a + b;
        return a;
    }

    Pos& operator -= (Pos& a, const Pos& b)
    {
        a = a - b;
        return a;
    }

    bool operator == (const Pos& a, const Pos& b)
    {
        bool xs = false;
        bool ys = false;
        bool zs = false;
        bool as = false;

        xs = ((int)a.x == (int)b.x);
        ys = ((int)a.y == (int)b.y);
        zs = ((int)a.z == (int)b.z);
        as = ((int)a.angle == (int)b.angle);

        return (xs && ys && zs && as);
    }

    bool operator != (const Pos& a, const Pos& b)
    {
        return !(a == b);
    }

    std::ostream& operator << (std::ostream& o, const Pos& p)
    {
        o << "X: " << p.x << ", ";
        o << "Y: " << p.y << ", ";
        o << "Z: " << p.z << ", ";
        o << "Angle: " << p.angle;

        return o;
    }
}