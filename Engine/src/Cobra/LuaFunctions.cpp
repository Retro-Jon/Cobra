#include "headers/LuaFunctions.hpp"

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
    Cobra::DeleteAllObjects();
    glfwSetWindowShouldClose(Cobra::window->GetScreen(), GL_TRUE);
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
        Cobra::window->MoveCamera(Cobra::objects[idx]->GetBoundCamera(), f);
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
        Cobra::window->PushCamera(Cobra::objects[idx]->GetBoundCamera(), f);
    }

    return 0;
}

int BindToCamera(lua_State* L)
{
    lua_getglobal(L, "InstanceID");
    int idx = lua_tointeger(L, -1);

    std::string cam = lua_tostring(L, 1);

    Cobra::objects[idx]->BindToCamera(cam);
    return 0;
}

int TriggerEvent(lua_State* L)
{
    std::string e = lua_tostring(L, 1);

    for (std::pair<int, Cobra::Object*> o : Cobra::objects)
    {
        o.second->Event(e);
    }
    return 0;
}

int CreateCamera(lua_State* L)
{
    std::string name = lua_tostring(L, 1);

    Cobra::Pos p;
    p.x = (double)lua_tonumber(L, 2);
    p.y = (double)lua_tonumber(L, 3);
    p.z = (double)lua_tonumber(L, 4);
    p.horizontal = (double)lua_tointeger(L, 5);
    p.vertical = (double)lua_tonumber(L, 6);

    Cobra::window->CreateNewCamera(name, p);

    return 0;
}

int CreateObject(lua_State* L)
{
    std::string name = lua_tostring(L, 1);
    Cobra::CreateObject(name);
    return 0;
}