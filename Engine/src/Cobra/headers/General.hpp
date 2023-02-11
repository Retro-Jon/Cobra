#pragma once

#include "Core.hpp"
#define _USE_MATH_DEFINES
#define LUA_USE_APICHECK
#include <cmath>
#include <ostream>
#include <iostream>
#include <lua/include/lua.hpp>
#include <type_traits>

#include "Sectors.hpp"

namespace Cobra
{
    struct Math
    {
        double sin[360];
        double cos[360];
        double tan[360];
        double rad[360];
    };

    COBRA_API extern struct Math M;

    static void PopulateMath()
    {
        for (int i = 0; i < 360; i++)
        {
            double v = ((double)i) * (M_PI / 180);
            M.rad[i] = v;

            M.sin[i] = std::sin(v);
            M.cos[i] = std::cos(v);
            M.tan[i] = std::tan(v);
        }
    }

    COBRA_API extern double ElapsedTime;

    COBRA_API extern void CalculateElapsedTime();

    COBRA_API extern int RoundToInt(double num);
    COBRA_API extern int RoundToInt(double num, double comp);

    template <typename T, typename T2, typename T3>
    static void Clamp(T& value, const T2& lower, const T3& upper, bool reset = true)
    {
        if (reset)
        {
            T difference = upper - lower;

            if (value > upper - 1)
                value -= difference;
            
            if (value < lower)
                value += difference;
        } else {
            if (value > upper)
                value = upper;
            
            if (value < lower)
                value = lower;
        }
    }
    
    static double TriArea(const double& x1, const double& y1, const double& x2, const double& y2, const double& x3, const double& y3)
    {
        return std::abs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 -y2)) / 2.0);
    }

    static bool IsInTri(const double& px, const double& py, const double& x1, const double& y1, const double& x2, const double& y2, const double& x3, const double& y3)
    {
        double A = TriArea(x1, y1, x2, y2, x3, y3);
        double A1 = TriArea(px, py, x2, y2, x3, y3);
        double A2 = TriArea(x1, y1, px, py, x3, y3);
        double A3 = TriArea(x1, y1, x2, y2, px, py);

        return (A == A1 + A2 + A3);
    }

    static bool IsInRect(const double& px, const double& py, const double& x1, const double& y1, const double& x2, const double& y2, const double& x3, const double& y3, const double& x4, const double y4)
    {
        return (IsInTri(px, py, x1, y1, x2, y2, x3, y3) || IsInTri(px, py, x3, y3, x4, y4, x1, y1));
    }

    static std::pair<double, double> LineGraph(const double& x, const double& y, const double& x1, const double& y1, const double& x2, const double& y2)
    {
        double m = (y2 - y1) / (x2 - x1);
        double b = y2 - m * x2;
        double xi = -((y - m * x) / m);
        double yi = b;

        return std::pair<double, double>(xi, yi);
    }

    COBRA_API extern double dist(double x1, double y1, double x2, double y2);

    template <typename T>
    struct Data
    {
        T v;
    };

    template<class T>
    static T LuaGetTableNumberValue(lua_State* L, const char* key)
    {
        T res;
        lua_pushstring(L, key);
        lua_gettable(L, -2);
        
        res = (T)lua_tonumber(L, -1);

        lua_pop(L, 1);
        return res;
    }

    static const char* LuaGetTableStringValue(lua_State* L, const char* key)
    {
        const char* res;
        lua_pushstring(L, key);
        lua_gettable(L, -2);
        
        res = lua_tostring(L, -1);

        lua_pop(L, 1);
        return res;
    }

    static const bool LuaGetTableBoolValue(lua_State* L, const char* key)
    {
        bool res;
        lua_pushstring(L, key);
        lua_gettable(L, -2);

        res = lua_toboolean(L, -1);
        
        lua_pop(L, 1);
        return res;
    }

    Pos operator + (const Pos& a, const Pos& b);
    Pos operator - (const Pos& a, const Pos& b);
    Pos& operator += (Pos& a, const Pos& b);
    Pos& operator -= (Pos& a, const Pos& b);

    bool operator == (const Pos& a, const Pos& b);
    bool operator != (const Pos& a, const Pos& b);

    std::ostream& operator << (std::ostream& o, const Pos& p);
    std::ostream& operator << (std::ostream& o, const SectorWall& w);

    void Log(const char* message);
    void ERROR(const char* message);
    void Exit();
}