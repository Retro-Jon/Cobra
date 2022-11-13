#include "General.hpp"
#include "Application.hpp"
#include "../lua/include/lua.hpp"
#include "LuaFunctions.hpp"

#include <iostream>
#include <iomanip>

namespace Engine
{
    void Log(const char* message)
    {
        std::cout << message << std::endl;
    }

    bool LuaCheck(lua_State* L, int r)
    {
        if (r != LUA_OK)
        {
            std::string errormsg = lua_tostring(L, -1);
            std::cout << "[LUA] " << errormsg << std::endl;
            return false;
        }
        return true;
    }

    void LuaCallScript(lua_State *L, const char* function, std::vector<LuaArg> args)
    {
        lua_getglobal(L, function);

        if (lua_isfunction(L, -1))
        {
            for (LuaArg a : args)
            {
                if (a.GetType() == 0)
                    lua_pushstring(L, std::string(a).c_str());
                else if (a.GetType() == 1)
                    lua_pushnumber(L, (float)a);
                else if (a.GetType() == 2)
                    lua_pushboolean(L, (bool)a);
            }

            LuaCheck(L, lua_pcall(L, args.size(), 0, 0));
        }
    }

    lua_State* CreateLuaState(const char* script)
    {
        lua_State* L = luaL_newstate();

        luaopen_base(L);
        luaopen_coroutine(L);
        luaopen_math(L);
        luaopen_string(L);
        luaopen_package(L);
        luaopen_table(L);
        luaopen_utf8(L);

        lua_register(L, "Test", lua_Test);
        lua_register(L, "Quit", lua_Quit);
        lua_register(L, "ReturnTest", lua_ReturnTest);

        bool res = LuaCheck(L, luaL_dofile(L, script));

        return L;
    }
}
