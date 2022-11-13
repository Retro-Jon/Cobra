#pragma once

#include "Core.hpp"
#include "../lua/include/lua.hpp"
#include <vector>

namespace Engine
{
    class Application;
    
    class API Component
    {
        protected:
            lua_State* script;
            std::vector<Component*> children;
            const char* name;
            Component* parent;
        
        public:
            Component();
            Component(const char* Name);
            Component(const char* Name, const char* ScriptPath);
            Component(const char* Name, Component* Parent);
            Component(const char* Name, Component* Parent, const char* ScriptPath);
            ~Component();

            void AddChild(const char* Name);
            void AddChild(const char* Name, const char* ScriptPath);
            Component* GetRoot();
            
            void Logic();
    };
}
