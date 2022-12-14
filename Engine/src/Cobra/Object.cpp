#include "headers/Object.hpp"
#include <iostream>

void LuaRegisterFunctions(lua_State* L);

namespace Cobra
{
    std::map<int, Object*> objects;
    int next_object_id = 0;

    bool CheckLua(lua_State* L, int r, const char* function_name)
    {
        if (r != LUA_OK && lua_isnil(L, 0))
        {
            std::string errormsg = lua_tostring(L, -1);
            std::cout << "[LUA] ERROR at " << function_name << ":\n" << errormsg << "\n" << std::endl;
            return false;
        }
        return true;
    }

    Object::Object(int index)
    {
        idx = index;
        position = (Pos){.x = 0, .y = 0, .z = 0, .horizontal = 0, .vertical = 0};
        bound_camera = "";
    }

    void Object::Ready(std::string ScriptPath)
    {
        if (ScriptPath != "")
        {
            script = luaL_newstate();

            lua_pushnumber(script, idx);
            lua_setglobal(script, "InstanceID");

            luaL_dofile(script, ScriptPath.c_str());

            luaL_openlibs(script);
            LuaRegisterFunctions(script);

            lua_getglobal(script, "OnReady");
            CheckLua(script, lua_pcall(script, 0, 0, 0), "OnReady");
        }
    }

    Object::~Object()
    {
        lua_getglobal(script, "OnDelete");
        CheckLua(script, lua_pcall(script, 0, 0, 0), "OnDelete");
        lua_close(script);
    }

    void Object::KeyInput(int key, int action)
    {
        lua_getglobal(script, "KeyInput");
        lua_pushnumber(script, key);
        lua_pushnumber(script, action);
        CheckLua(script, lua_pcall(script, 2, 0, 0), "KeyInput");
    }

    void Object::Logic()
    {
        lua_getglobal(script, "Logic");
        CheckLua(script, lua_pcall(script, 0, 0, 0), "Logic");
    }

    void Object::Move(Pos direction)
    {
        position = direction;
    }

    void Object::Push(Pos force)
    {
        position.horizontal += force.horizontal * ElapsedTime;
        position.vertical += force.vertical * ElapsedTime;

        Clamp(position.horizontal, 0, 360);
        Clamp(position.vertical, 0, 360);

        double COS = M.cos[(int)position.horizontal] * ElapsedTime;
        double SIN = M.sin[(int)position.horizontal] * ElapsedTime;

        if (force.x != 0)
        {
            if (M.tan[(int)position.horizontal] <= 90 || M.tan[(int)position.horizontal] >= 270)
            {
                position.x += force.x * COS;
                position.y -= force.x * SIN;
            } else {
                position.x -= force.x * COS;
                position.y += force.x * SIN;
            }
        }

        if (force.y != 0)
        {
            position.x += force.y * SIN;
            position.y += force.y * COS;
        }

        position.z += force.z * ElapsedTime;
    }

    void Object::BindToCamera(std::string camera)
    {
        bound_camera = camera;
    }

    std::string Object::GetBoundCamera()
    {
        return bound_camera;
    }
}
