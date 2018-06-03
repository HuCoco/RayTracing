#pragma once
#include <Platform/Platform.h>
#include <Math/Math.h>
#include <Math/Vec2f.h>
#include <Math/Vec3f.h>


Vec3f ApproximateSpecularIBL(Vec3f SpecularColor, float Roughness, Vec3f N, Vec3f V);