/*
 * Dawn Engine
 * Written by David Avedissian (c) 2012-2016 (git@davedissian.com)
 */
#pragma once

#define MAX_COMPONENTS 256

typedef std::bitset<MAX_COMPONENTS> ComponentBitset;

class ComponentTypeManager
{
public:
    template <class T>
    static ComponentBitset& getTypeFor()
    {
        assert(std::is_base_of<Component, T>::value == true);
        auto it = componentTypes.get(typeid(T));
        if (it == componentTypes.end()) {
            componentTypes[typeid(T)] = ComponentBitset(1 << nextComponentId++);
            return componentTypes[typeid(T)];
        } else {
            return *it;
        }
    }

private:
    static TypeMap<ComponentBitset*> componentTypes;
    static int nextComponentId;
};
