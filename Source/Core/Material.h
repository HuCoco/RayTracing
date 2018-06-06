#pragma once
#include <Math/Vec3f.h>

class Material
{
public:
    Vec3f k_a;
    Vec3f k_d;
    Vec3f k_r;

    float n; // The specular reflection exponent. It ranges from 0.0 to 128.0. 

    Vec3f k_rg;
};