#include "headers/LuaFunctions.hpp"
#include "headers/Window.hpp"
#include "headers/Renderer.hpp"
#include "headers/Object.hpp"
#include "headers/Core.hpp"
#include "headers/General.hpp"

int DestroyObject(lua_State* L)
{
    lua_getglobal(L, "InstanceID");
    int idx = lua_tointeger(L, -1);
    lua_pop(L, -1);

    Cobra::DeleteObject(idx);
    return 0;
}

int Quit(lua_State* L)
{
    Cobra::Exit();
    return 0;
}

int Move(lua_State* L)
{
    lua_getglobal(L, "InstanceID");
    int idx = lua_tointeger(L, -1);

    Cobra::Pos f;

    f.x = (double)lua_tonumber(L, 1);
    f.y = (double)lua_tonumber(L, 2);
    f.z = (double)lua_tonumber(L, 3);
    f.horizontal = (double)lua_tonumber(L, 4);
    f.vertical = (double)lua_tonumber(L, 5);

    Cobra::objects[idx]->Move(f);

    if (Cobra::objects[idx]->GetBoundCamera() != "")
    {
        Cobra::renderer->MoveCamera(Cobra::objects[idx]->GetBoundCamera(), f);
    }
    return 0;
}

int Push(lua_State* L)
{
    lua_getglobal(L, "InstanceID");
    int idx = lua_tointeger(L, -1);

    Cobra::Pos f;

    f.x = (double)lua_tonumber(L, 1);
    f.y = (double)lua_tonumber(L, 2);
    f.z = (double)lua_tonumber(L, 3);
    f.horizontal = (double)lua_tonumber(L, 4);
    f.vertical = (double)lua_tonumber(L, 5);
    
    Cobra::objects[idx]->Push(f);

    if (Cobra::objects[idx]->GetBoundCamera() != "")
    {
        Cobra::renderer->PushCamera(Cobra::objects[idx]->GetBoundCamera(), f);
    }

    return 0;
}

int BindToCamera(lua_State* L)
{
    lua_getglobal(L, "InstanceID");
    int idx = lua_tointeger(L, -1);

    const char* cam = lua_tostring(L, 1);

    Cobra::objects[idx]->BindToCamera(cam);
    return 0;
}

int TriggerEvent(lua_State* L)
{
    const char* e = lua_tostring(L, 1);

    for (std::pair<int, Cobra::Object*> o : Cobra::objects)
    {
        o.second->Event(e);
    }
    return 0;
}

int CreateCamera(lua_State* L)
{
    const char* name = lua_tostring(L, 1);

    Cobra::Camera c;
    c.position.x = (double)lua_tonumber(L, 2);
    c.position.y = (double)lua_tonumber(L, 3);
    c.position.z = (double)lua_tonumber(L, 4);
    c.position.horizontal = (double)lua_tointeger(L, 5);
    c.position.vertical = (double)lua_tonumber(L, 6);
    c.fov = lua_tointeger(L, 7);

    Cobra::renderer->CreateNewCamera(name, c);

    return 0;
}

int CreateObject(lua_State* L)
{
    const char* name = lua_tostring(L, 1);
    Cobra::CreateObject(name);
    return 0;
}