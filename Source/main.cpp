#pragma once
#include "Core/BRDF.h"

int main()
{
    auto a = ApproximateSpecularIBL(Vec3f(1.0f,1.0f,1.0f), 1.0f, Vec3f(0.0f,1.0f,0.0f), Vec3f(0.5f,0.5f,0.0f));
    return 0;
}