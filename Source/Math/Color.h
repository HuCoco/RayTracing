// Copyright 2017-2018 Hu Ke. All rights reserved.
#pragma once
#include <Platform/Platform.h>
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

    FColor(const UColor& c)
        : r(static_cast<float>(c.r) / 255.0f)
        , g(static_cast<float>(c.g) / 255.0f)
        , b(static_cast<float>(c.b) / 255.0f)
        , a(static_cast<float>(c.a) / 255.0f)
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
    UColor(float cr, float cg, float cb, float ca)
        : r(cr)
        , g(cg)
        , b(cb)
        , a(ca)
    {

    }

    UColor(const UColor& c)
        : r(c.r)
        , g(c.g)
        , b(c.b)
        , a(c.a)
    {

    }

    UColor(const FColor& c)
        : r(static_cast<uint8_t>(c.r * 255.0f))
        , g(static_cast<uint8_t>(c.g * 255.0f))
        , b(static_cast<uint8_t>(c.b * 255.0f))
        , a(static_cast<uint8_t>(c.a * 255.0f))
    {

    }

public:
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};