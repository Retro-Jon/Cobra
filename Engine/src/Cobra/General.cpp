#include "headers/General.hpp"
#include <iostream>
#include <chrono>

namespace Cobra
{
    Math M;

    Pos operator + (const Pos& a, const Pos& b)
    {
        Pos res;
        res.x = a.x + b.x;
        res.y = a.y + b.y;
        res.z = a.z + b.z;
        res.horizontal = a.horizontal + b.horizontal;
        res.vertical = a.vertical + b.vertical;

        Clamp(res.horizontal, 0, 360);
        Clamp(res.vertical, 0, 360);
        
        return res;
    }

    Pos operator - (const Pos& a, const Pos& b)
    {
        Pos res;
        res.x = a.x - b.x;
        res.y = a.y - b.y;
        res.z = a.z - b.z;
        res.horizontal = a.horizontal - b.horizontal;
        res.vertical = a.vertical - b.vertical;

        Clamp(res.horizontal, 0, 360);
        Clamp(res.vertical, 0, 360);
        
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
        bool hs = false;
        bool vs = false;

        xs = ((int)a.x == (int)b.x);
        ys = ((int)a.y == (int)b.y);
        zs = ((int)a.z == (int)b.z);
        hs = ((int)a.horizontal == (int)b.horizontal);
        vs = ((int)a.vertical == (int)b.vertical);

        return (xs && ys && zs && hs && vs);
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
        o << "Horizontal: " << p.horizontal, ", ";
        o << "Vertical: " << p.vertical;

        return o;
    }

    double ElapsedTime;

    void CalculateElapsedTime()
    {
        static auto tp1 = std::chrono::system_clock::now();
        static auto tp2 = std::chrono::system_clock::now();

        tp2 = std::chrono::system_clock::now();

        std::chrono::duration<double> ep = tp2 - tp1;

        tp1 = tp2;

        ElapsedTime = ep.count();
    }

    int RoundToInt(double num)
    {
        int res;

        if (num - (int)num < 0.5)
            res = (int)num;
        else
            res = (int)num + 1;
        
        return res;
    }

    int RoundToInt(double num, double comp)
    {
        int res;

        if (num > comp)
            res = (int)num + 1;
        else
            res = (int)num;
        
        return res;
    }

    double dist(double x1, double y1, double x2, double y2)
    {
        return std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    }
}