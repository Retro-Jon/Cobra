#include "headers/Object.hpp"
#include <iostream>
#include <thread>

void LuaRegisterFunctions(lua_State* L);

void dumpstack (lua_State *L) {
  int top=lua_gettop(L);
  for (int i=1; i <= top; i++) {
    printf("%d\t%s\t", i, luaL_typename(L,i));
    switch (lua_type(L, i)) {
      case LUA_TNUMBER:
        printf("%g\n",lua_tonumber(L,i));
        break;
      case LUA_TSTRING:
        printf("%s\n",lua_tostring(L,i));
        break;
      case LUA_TBOOLEAN:
        printf("%s\n", (lua_toboolean(L, i) ? "true" : "false"));
        break;
      case LUA_TNIL:
        printf("%s\n", "nil");
        break;
      default:
        printf("%p\n",lua_topointer(L,i));
        break;
    }
  }
}

namespace Cobra
{
    std::map<int, Object*> objects;
    std::queue<Object*> DeletionQueue;
    int next_object_id = 0;

    // Check if lua function returns an error
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
        queued = false;
    }

    void Object::Ready(std::string ScriptPath)
    {
        if (queued) return;
        
        if (ScriptPath != "")
        {
            script = luaL_newstate();
            lua_pushnumber(script, idx);
            lua_setglobal(script, "InstanceID");

            luaL_dofile(script, ScriptPath.c_str());

            luaL_openlibs(script);
            LuaRegisterFunctions(script);
            
            std::string funcs[] = {"OnReady", "KeyInput", "Logic", "Event"};

            for (std::string f : funcs)
            {
                lua_getglobal(script, f.c_str());

                script_functions[f] = false;

                if (lua_isfunction(script, 1))
                    script_functions[f] = true;
                
                lua_pop(script, 1);
            }

            if (has_function("OnReady"))
            {
                lua_getglobal(script, "OnReady");

                if (lua_isfunction(script, 1))
                    CheckLua(script, lua_pcall(script, 0, 0, 0), "OnReady");
                else
                    lua_pop(script, 1);
            }
        }
    }

    Object::~Object()
    {
        lua_close(script);
    }

    void Object::Delete()
    {
        if (queued) return;

        queued = true;
        
        lua_getglobal(script, "OnDelete");
        
        if (lua_isfunction(script, 1))
            CheckLua(script, lua_pcall(script, 0, 0, 0), "OnDelete");
        else
            lua_pop(script, 1);

        DeletionQueue.push(this);
    }

    bool Object::has_function(std::string name)
    {
        return script_functions[name];
    }

    void Object::KeyInput(int key, int action)
    {
        if (queued) return;

        if (has_function("KeyInput"))
        {
            lua_getglobal(script, "KeyInput");

            if (lua_isfunction(script, 1))
            {
                lua_pushnumber(script, key);
                lua_pushnumber(script, action);

                CheckLua(script, lua_pcall(script, 2, 0, 0), "KeyInput");
            } else
                lua_pop(script, 1);
        }
    }

    void Object::Logic()
    {
        if (queued) return;

        if (has_function("Logic"))
        {
            lua_getglobal(script, "Logic");

            if (lua_isfunction(script, 1))
                CheckLua(script, lua_pcall(script, 0, 0, 0), "Logic");
        }
    }

    void Object::Event(std::string e)
    {
        if (queued) return;

        if (has_function("Event"))
        {
            lua_getglobal(script, "Event");
            
            if (lua_isfunction(script, 1))
            {
                lua_pushstring(script, e.c_str());
                
                if (script != nullptr)
                {
                    int r = lua_pcall(script, 1, 0, 0);
                    CheckLua(script, r, "Event");
                }
            }
        }
    }

    void Object::Move(Pos direction)
    {
        if (queued) return;

        position = direction;
    }

    void Object::Push(Pos force)
    {
        if (queued) return;

        position.horizontal += force.horizontal * ElapsedTime;
        position.vertical += force.vertical * ElapsedTime;

        Clamp(position.horizontal, 0, 360);
        Clamp(position.vertical, 0, 360);

        double COS = std::cos(position.horizontal) * ElapsedTime;
        double SIN = std::sin(position.horizontal) * ElapsedTime;
        double TAN = std::tan(position.horizontal);

        if (force.x != 0)
        {
            if (TAN <= 90 || TAN >= 270)
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
        if (queued) return;

        bound_camera = camera;
    }

    std::string Object::GetBoundCamera()
    {
        return bound_camera;
    }
}
