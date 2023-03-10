#include "headers/General.hpp"
#include "headers/Object.hpp"
#include "headers/Window.hpp"
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

    bool operator == (const Color& a, const Color& b)
    {
        bool rs = false;
        bool gs = false;
        bool bs = false;
        bool as = false;

        rs = ((int)a.r == (int)b.r);
        gs = ((int)a.g == (int)b.g);
        bs = ((int)a.b == (int)b.b);
        as = ((int)a.a == (int)b.a);

        return (rs && gs && bs && as);
    }

    bool operator != (const Color& a, const Color& b)
    {
        return !(a == b);
    }

    bool operator == (const SectorPoint& a, const SectorPoint& b)
    {
        return ((a.x == b.x) && (a.y == b.y));
    }

    bool operator != (const SectorPoint& a, const SectorPoint& b)
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

    std::ostream& operator << (std::ostream& o, const SectorWall& w)
    {
        o << "X1: " << w.p1.x << " Y1: " << w.p1.y << " X2: " << w.p2.x << " Y2: " << w.p2.y;
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
        if (num - (int)num < 0.5)
            return (int)num;
        else
            return (int)num + 1;
    }

    int RoundToInt(double num, double comp)
    {
        if (num > comp)
            return (int)num + 1;
        else
            return (int)num;
    }

    double dist(double x1, double y1, double x2, double y2)
    {
        double x = x2 - x1;
        double y = y2 - y1;

        return std::sqrt((x * x) + (y * y));
    }

    void Log(const char* message)
    {
        std::cout << "Cobra Log " << message << std::endl;
    }

    void ERROR(const char* message)
    {
        std::cerr << "Cobra ERROR " << message << std::endl;
        Exit();
    }

    void Exit()
    {
        Cobra::DeleteAllObjects();
        glfwSetWindowShouldClose(Cobra::window->GetScreen(), GL_TRUE);
    }
}