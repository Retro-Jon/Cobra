#pragma once

#include "Core.hpp"
#include <string>
#include "../lua/include/lua.hpp"
#include <vector>

namespace Engine
{
    class Component;
    class Application;
    
    static Engine::Application* app;
    
    class API LuaArg
    {
        private:
            std::string s;
            float num;
            bool b;
            int type;
        
        public:
            LuaArg(const char* v)
            {
                s = std::string(v);
                type = 0;
            }
            LuaArg(int v)
            {
                num = (float)v;
                type = 1;
            }
            LuaArg(float v)
            {
                num = v;
                type = 1;
            }
            LuaArg(double v)
            {
                num = (float)v;
                type = 1;
            }
            LuaArg(bool v)
            {
                b = v;
                type = 2;
            }

            int GetType() { return type; }

            operator std::string() const { return s; }
            operator int() const { return (int)num; }
            operator float() const { return num; }
            operator double() const { return (double)num; }
            operator bool() const { return b; }
    };

    API void Log(const char* message);
    API lua_State* CreateLuaState(const char* script);
    API void LuaCallScript(lua_State* L, const char* function, std::vector<LuaArg> args = {});
    API lua_State* CreateLuaState(const char* script);
}
