#pragma once

#include "Core.hpp"
#include "Object.hpp"
#include "General.hpp"
#include "Window.hpp"
#include "../../lua/include/lua.hpp"

int DestroyObject(lua_State* L);
int Quit(lua_State* L);
int Move(lua_State* L);
int Push(lua_State* L);
int BindToCamera(lua_State* L);

int CreateCamera(lua_State* L);

void LuaRegisterFunctions(lua_State* L)
{
    lua_register(L, "DestroyObject", DestroyObject);
    lua_register(L, "Quit", Quit);
    lua_register(L, "Move", Move);
    lua_register(L, "Push", Push);
    lua_register(L, "BindToCamera", BindToCamera);
    
    lua_register(L, "CreateCamera", CreateCamera);
}