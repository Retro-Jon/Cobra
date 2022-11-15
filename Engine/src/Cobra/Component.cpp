#include "headers/Component.hpp"
#include <iostream>

namespace Cobra
{
    bool CheckLua(lua_State* L, int r)
    {
        if (r != LUA_OK)
        {
            std::string errormsg = lua_tostring(L, -1);
            std::cout << errormsg << std::endl;
            return false;
        }
        return true;
    }

    Component::Component(const char* Name)
    {
        name = Name;
        parent = NULL;
    }

    Component::Component(const char* Name, const char* ScriptPath)
    {
        name = Name;
        parent = NULL;

        L = luaL_newstate();
        luaL_dofile(L, ScriptPath);

        luaL_openlibs(L);

        lua_getglobal(L, "Ready");
        CheckLua(L, lua_pcall(L, 0, 0, 0));
    }

    Component::Component(const char* Name, Component* Parent)
    {
        name = Name;
        parent = Parent;
    }
    
    Component::Component(const char* Name, const char* ScriptPath, Component* Parent)
    {
        Component(Name, ScriptPath);
        parent = Parent;
    }

    Component::~Component()
    {
        lua_close(L);

        for (Component* C : children)
        {
            delete C;
        }
    }

    void Component::AddChild(Component* Child)
    {
        Child->SetParent(this);
        children.push_back(Child);
    }
}
