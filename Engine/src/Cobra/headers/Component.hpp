#pragma once

#include "../../lua/include/lua.hpp"
#include <vector>
#include "Core.hpp"

namespace Cobra
{
    class COBRA_API Component
    {
        protected:
            lua_State* L;
            std::vector<Component*> children;
            Component* parent;
            const char* name;
        
        public:
            Component(const char* Name);
            Component(const char* Name, const char* ScriptPath);

            Component(const char* Name, Component* Parent);
            Component(const char* Name, const char* ScriptPath, Component* Parent);

            ~Component();

            inline lua_State* getLuaState() { return this->L; }
            inline Component* GetChild(int idx) { return this->children[idx]; }
            inline Component* GetParent() { return this->parent; }
            inline void SetParent(Component* NewParent) { parent = NewParent; }
            
            void AddChild(Component* Child);
    };
}