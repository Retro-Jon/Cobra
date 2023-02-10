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

namespace Cobra
{
    class COBRA_API Object
    {
        private:
            Pos position;
            int idx;
            std::string bound_camera;
            std::map<std::string, bool> script_functions;
            bool queued;
        
        protected:
            lua_State* script;
        
        public:
            Object(int index = -1);
            void Ready(std::string ScriptPath = "");
            ~Object();
            void Delete();

            bool has_function(std::string name);

            //lua functions
            void KeyInput(int key, int action);
            void Logic();
            void Event(std::string e);

            // lua and C++
            void Move(Pos direction);
            void Push(Pos direction);
            void BindToCamera(std::string camera);

            std::string GetBoundCamera();
    };

    COBRA_API extern std::map<int, Object*> objects;
    COBRA_API extern std::queue<Object*> DeletionQueue;
    COBRA_API extern int next_object_id;

    static int CreateObject(std::string ScriptPath = "")
    {
        next_object_id++;
        objects.insert(std::pair<int, Object*>(next_object_id, new Object(next_object_id)));
        objects[next_object_id]->Ready("scripts/" + ScriptPath);
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
            std::cout << o.first << " : " << o.second << std::endl;
            DeleteObject(o.first);
        }
    }
}
