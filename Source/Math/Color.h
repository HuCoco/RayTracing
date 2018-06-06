// Copyright 2017-2018 Hu Ke. All rights reserved.
#pragma once
#include <Platform/Platform.h>
#include <Math/Vec3f.h>
class FColor;
class UColor;

class FColor
{
public:
    FColor(float cr, float cg, float cb, float ca)
        : r(cr)
        , g(cg)
        , b(cb)
        , a(ca)
    {

    }

    FColor(const FColor& c)
        : r(c.r)
        , g(c.g)
        , b(c.b)
        , a(c.a)
    {

    }


    

public:
    float r;
    float g;
    float b;
    float a;
};


class UColor
{
public:
    UColor() = default;


    UColor(const UColor& c)
        : r(c.r)
        , g(c.g)
        , b(c.b)
        , a(c.a)
    {

    }

    UColor(const Vec3f& c)
        : r(static_cast<uint8_t>(c.x * 255.0f))
        , g(static_cast<uint8_t>(c.y * 255.0f))
        , b(static_cast<uint8_t>(c.z * 255.0f))
        , a(static_cast<uint8_t>(255))
    {

    }

    void operator=(const Vec3f& c)
    {
        r = (static_cast<uint8_t>(c.x * 255.0f));
        g = (static_cast<uint8_t>(c.y * 255.0f));
        b = (static_cast<uint8_t>(c.z * 255.0f));
        a = (static_cast<uint8_t>(255));
    }

public:
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};