#include "LuaFunctions.hpp"
#include "General.hpp"
#include "Application.hpp"

#include <iostream>

namespace Engine
{
    int lua_Test(lua_State* L)
    {
        Log("[LUA] Test");
        return 0;
    }

    int lua_Quit(lua_State* L)
    {
        Log("[LUA] Quit");
        app->Quit();

        return 0;
    }

    int lua_ReturnTest(lua_State* L)
    {
        Log("[LUA] ReturnTest");

        std::string t = app->GetTitle();
        std::cout << t << std::endl;

        lua_pushstring(L, "");
        return 1;
    }
}
