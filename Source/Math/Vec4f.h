#pragma once
#include <Math/Accel/MathAccel.h>
#include <Math/Vec.h>

using Vec4f = Vec<float, 4>;


template <>
class Vec<float, 4>
{
public:
    inline Vec(const SIMD4F& v)
    {
        SIMD4F_USTORE3(v, this->Data());
    }

public:
    inline Vec()
        : x(0.0f)
        , y(0.0f)
        , z(0.0f)
        , w(0.0f)
    {

    }

    inline Vec(const Vec<float, 4>& v)
        : x(v.x)
        , y(v.y)
        , z(v.z)
        , w(v.w)
    {

    }

    inline Vec(float vx, float vy, float vz,float vw)
        : x(vx)
        , y(vy)
        , z(vz)
        , w(vw)
    {

    }

    inline Vec(float* v)
        : x(v[0])
        , y(v[1])
        , z(v[2])
        , w(v[3])
    {

    }

    inline float* Data()
    {
        return &x;
    }

    inline const float* Data() const
    {
        return &x;
    }

    inline float& operator()(uint32_t i)
    {
        ASSERT_MSG(i >= 0 && i < 3, "Over range Vec2.");
        return *(reinterpret_cast<float*>(&x) + i);
    }

    inline const float& operator()(uint32_t i) const
    {
        ASSERT_MSG(i >= 0 && i < 3, "Over range Vec2.");
        return *(reinterpret_cast<const float*>(&x) + i);
    }

    inline float& operator[](uint32_t i)
    {
        ASSERT_MSG(i >= 0 && i < 3, "Over range Vec2.");
        return *(reinterpret_cast<float*>(&x) + i);
    }

    inline const float& operator[](uint32_t i) const
    {
        ASSERT_MSG(i >= 0 && i < 3, "Over range Vec2.");
        return *(reinterpret_cast<const float*>(&x) + i);
    }

    inline Vec<float, 4> operator-() const
    {
        return Vec<float, 4>(SIMD4F_SUB(SIMD4F_ZERO(), SIMD4F_ULOAD4(this->Data())));
    }

    inline Vec<float, 4> operator*(const Vec<float, 4>& v) const
    {
        return Vec<float, 4>(SIMD4F_MUL(SIMD4F_ULOAD4(this->Data()), SIMD4F_ULOAD4(v.Data())));
    }

    inline Vec<float, 4> operator/(const Vec<float, 4>& v) const
    {
        return Vec<float, 4>(SIMD4F_DIV(SIMD4F_ULOAD4(this->Data()), SIMD4F_ULOAD4(v.Data())));
    }

    inline Vec<float, 4> operator+(const Vec<float, 4>& v) const
    {
        return Vec<float, 4>(SIMD4F_ADD(SIMD4F_ULOAD4(this->Data()), SIMD4F_ULOAD4(v.Data())));
    }

    inline Vec<float, 4> operator-(const Vec<float, 4>& v) const
    {
        return Vec<float, 4>(SIMD4F_SUB(SIMD4F_ULOAD4(this->Data()), SIMD4F_ULOAD4(v.Data())));
    }

    inline Vec<float, 4> operator*(float s) const
    {
        return Vec<float, 4>(SIMD4F_MUL(SIMD4F_ULOAD4(this->Data()), SIMD4F_SET(s)));
    }

    inline Vec<float, 4> operator/(float s) const
    {
        return Vec<float, 4>(SIMD4F_DIV(SIMD4F_ULOAD4(this->Data()), SIMD4F_SET(s)));
    }

    inline Vec<float, 4> operator+(float s) const
    {
        return Vec<float, 4>(SIMD4F_ADD(SIMD4F_ULOAD4(this->Data()), SIMD4F_SET(s)));
    }

    inline Vec<float, 4> operator-(float s) const
    {
        return Vec<float, 4>(SIMD4F_SUB(SIMD4F_ULOAD4(this->Data()), SIMD4F_SET(s)));
    }

    inline Vec<float, 4> operator*=(const Vec<float, 4>& v)
    {
        *this = SIMD4F_MUL(SIMD4F_ULOAD4(this->Data()), SIMD4F_ULOAD4(v.Data()));
        return *this;
    }

    inline Vec<float, 4> operator/=(const Vec<float, 4>& v)
    {
        *this = SIMD4F_DIV(SIMD4F_ULOAD4(this->Data()), SIMD4F_ULOAD4(v.Data()));
        return *this;
    }

    inline Vec<float, 4> operator+=(const Vec<float, 4>& v)
    {
        *this = SIMD4F_ADD(SIMD4F_ULOAD4(this->Data()), SIMD4F_ULOAD4(v.Data()));
        return *this;
    }

    inline Vec<float, 4> operator-=(const Vec<float, 4>& v)
    {
        *this = SIMD4F_SUB(SIMD4F_ULOAD4(this->Data()), SIMD4F_ULOAD4(v.Data()));
        return *this;
    }

    inline Vec<float, 4> operator*=(float s)
    {
        *this = SIMD4F_MUL(SIMD4F_ULOAD4(this->Data()), SIMD4F_SET(s));
        return *this;
    }

    inline Vec<float, 4> operator/=(float s)
    {
        *this = SIMD4F_MUL(SIMD4F_ULOAD4(this->Data()), SIMD4F_SET(s));
        return *this;
    }

    inline Vec<float, 4> operator+=(float s)
    {
        *this = SIMD4F_MUL(SIMD4F_ULOAD4(this->Data()), SIMD4F_SET(s));
        return *this;
    }

    inline Vec<float, 4> operator-=(float s)
    {
        *this = SIMD4F_MUL(SIMD4F_ULOAD4(this->Data()), SIMD4F_SET(s));
        return *this;
    }

    inline bool operator==(const Vec<float, 4>& v) const
    {
        for (int i = 0; i < 3; ++i)
        {
            if ((*this)[i] != v[i])
            {
                return false;
            }
        }
        return true;
    }

    inline bool operator!=(const Vec<float, 4>& v) const
    {
        return !operator==(v);
    }

    inline float GetLengthSquared() const
    {
        return SIMD4F_DOT4_SCALAR(SIMD4F_ULOAD4(this->Data()), SIMD4F_ULOAD4(this->Data()));
    }

    inline float GetLength() const
    {
        return SIMD4F_GET_X(SIMD4F_LENGTH4(SIMD4F_ULOAD4(this->Data())));
    }

    inline Vec<float, 4> GetNormalized() const
    {
        return Vec<float, 4>(SIMD4F_NORMALIZE4(SIMD4F_ULOAD4(this->Data())));
    }

    inline void Normalized()
    {
        *this = SIMD4F_NORMALIZE4(SIMD4F_ULOAD4(this->Data()));
    }

    static inline Vec4f Cross(const Vec4f& lhs, const Vec4f& rhs)
    {
        return Vec4f();
    }

    static inline float Dot(const Vec4f& lhs, const Vec4f& rhs)
    {
        return SIMD4F_DOT4_SCALAR(SIMD4F_ULOAD4(lhs.Data()), SIMD4F_ULOAD4(rhs.Data()));
    }

public:
    float x;
    float y;
    float z;
    float w;
};