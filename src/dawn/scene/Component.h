/*
 * Dawn Engine
 * Written by David Avedissian (c) 2012-2016 (git@davedissian.com)
 */
#pragma once

class DW_API Component
{
public:
    virtual ~Component() = 0;

protected:
    // Protected Constructor to prevent instantiation
    Component() {}
};
