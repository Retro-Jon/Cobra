#pragma once

#include "lua/include/lua.hpp"

namespace Engine
{
    int lua_Test(lua_State* L);
    int lua_Quit(lua_State* L);
    int lua_ReturnTest(lua_State* L);
}
