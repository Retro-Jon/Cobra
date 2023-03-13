#pragma once

#include "../../lua/include/lua.hpp"

int DestroyObject(lua_State* L);
int Quit(lua_State* L);
int Move(lua_State* L);
int Push(lua_State* L);
int ConnectCamera(lua_State* L);
int SetViewPortCamera(lua_State* L);
int TriggerEvent(lua_State* L);

int CreateCamera(lua_State* L);
int CreateViewPort(lua_State* L);

int CreateObject(lua_State* L);

int MoveCamera(lua_State* L);
int PushCamera(lua_State* L);

void LuaRegisterFunctions(lua_State* L)
{
    lua_register(L, "DestroyObject", DestroyObject);
    lua_register(L, "Quit", Quit);
    lua_register(L, "Move", Move);
    lua_register(L, "Push", Push);
    lua_register(L, "ConnectCamera", ConnectCamera);
    lua_register(L, "SetViewPortCamera", SetViewPortCamera);
    lua_register(L, "TriggerEvent", TriggerEvent);
    
    lua_register(L, "CreateCamera", CreateCamera);
    lua_register(L, "CreateViewPort", CreateViewPort);

    lua_register(L, "CreateObject", CreateObject);

    lua_register(L, "MoveCamera", MoveCamera);
    lua_register(L, "PushCamera", PushCamera);
}