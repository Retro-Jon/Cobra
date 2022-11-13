#include "Component.hpp"
#include "General.hpp"

namespace Engine
{
    Component::Component(){}

    Component::Component(const char* Name)
    {
        name = Name;
        parent = NULL;
    }

    Component::Component(const char* Name, const char* ScriptPath)
    {
        name = Name;
        parent = NULL;
        script = CreateLuaState(ScriptPath);

        LuaCallScript(script, "Ready");
    }

    Component::Component(const char* Name, Component* Parent)
    {
        name = Name;
        parent = Parent;
    }

    Component::Component(const char* Name, Component* Parent, const char* ScriptPath)
    {
        Component(Name, ScriptPath);
        parent = Parent;
    }

    Component::~Component()
    {
        LuaCallScript(script, "Close");

        for (Component* c : children)
        {
            delete c;
        }

        lua_close(script);
    }

    void Component::AddChild(const char* Name)
    {
        children.push_back(new Component(Name, this));
    }

    void Component::AddChild(const char* Name, const char* ScriptPath)
    {
        children.push_back(new Component(Name, this, ScriptPath));
    }

    Component* Component::GetRoot()
    {
        Component* c;

        if (parent != NULL) c = parent->GetRoot();
        else c = this;

        return c;
    }

    void Component::Logic()
    {
        LuaCallScript(script, "Logic");

        for (Component* c : children)
            c->Logic();
    }
}
