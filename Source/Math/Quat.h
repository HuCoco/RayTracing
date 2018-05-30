#pragma once
#include <Platform/Platform.h>
#include <Math/Accel/MathAccel.h>
#include <Math/Vec3f.h>

class Quat
{
public:
    inline Quat()
    {

    }

    inline Quat(const Quat& q)
        : s(q.s)
        , v(q.v)
    {

    }

    inline Quat(float qs,const Vec3f& qv)
        : s(qs)
        , v(qv)
    {

    }

    inline Quat Inverse() const
    {
        return Quat(s, -v);
    }

    inline Quat operator*(const Quat& q) const
    {
        return Quat(
            s * q.s - Vec3f::Dot(v, q.v),
            q.v * s + v * q.s + Vec3f::Cross(v, q.v));

    }

public:
    float s;
    Vec3f v;
};