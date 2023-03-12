#pragma once

#include "Core.hpp"
#include "Sectors.hpp"
#include "General.hpp"

#include "../../lua/include/lua.hpp"
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <queue>
#include <cstring>

namespace Cobra
{
    class COBRA_API Object
    {
        private:
            Pos position;
            int idx;
            const char* bound_camera;
            std::map<const char*, bool> script_functions;
            bool queued;
        
        protected:
            lua_State* script;
        
        public:
            Object(int index = -1);
            void Ready(const char* ScriptPath = "");
            ~Object();
            void Delete();

            bool has_function(const char* name);

            //lua functions
            void KeyInput(int key, int action);
            void Logic();
            void Event(const char* e);

            // lua and C++
            void Move(Pos direction);
            void Push(Pos direction);
            void BindToCamera(const char* camera);

            const char* GetBoundCamera();
    };

    COBRA_API extern std::map<int, Object*> objects;
    COBRA_API extern std::queue<Object*> DeletionQueue;
    COBRA_API extern int next_object_id;

    static int CreateObject(const char* ScriptPath = "")
    {
        next_object_id++;
        objects.insert(std::pair<int, Object*>(next_object_id, new Object(next_object_id)));
        std::string sp = "scripts/";
        sp += ScriptPath;

        objects[next_object_id]->Ready(sp.c_str());
        return objects.size();
    }

    static void DeleteObject(int index)
    {
        objects[index]->Delete();
    }

    static void DeleteAllObjects()
    {
        for (std::pair<int, Object*> o : objects)
        {
            DeleteObject(o.first);
        }
    }
}
