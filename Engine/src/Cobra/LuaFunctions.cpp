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

    f.x = (float)lua_tonumber(L, 1);
    f.y = (float)lua_tonumber(L, 2);
    f.z = (float)lua_tonumber(L, 3);
    f.angle = lua_tointeger(L, 4);

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

    f.x = (float)lua_tonumber(L, 1);
    f.y = (float)lua_tonumber(L, 2);
    f.z = (float)lua_tonumber(L, 3);
    f.angle = lua_tointeger(L, 4);

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

int CreateCamera(lua_State* L)
{
    std::string name = lua_tostring(L, 1);

    Cobra::Pos p;
    p.x = (float)lua_tonumber(L, 2);
    p.y = (float)lua_tonumber(L, 3);
    p.z = (float)lua_tonumber(L, 4);
    p.angle = lua_tointeger(L, 5);

    Cobra::window->CreateNewCamera(name, p);

    return 0;
}