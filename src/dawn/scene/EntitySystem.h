/*
 * Dawn Engine
 * Written by David Avedissian (c) 2012-2016 (git@davedissian.com)
 */
#pragma once

#include "scene/ComponentType.h"

class DW_API EntitySystem
{
public:
    EntitySystem() {}

    virtual void processEntity(Entity& e) = 0;

protected:
    template <class T>
    void addComponentType()
    {
        componentFlags |= ComponentTypeManager::getTypeFor<T>();
    }

private:
    ComponentBitset componentFlags;

};
