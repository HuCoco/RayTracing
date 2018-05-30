#pragma once
#include <Math/Accel/MathAccel.h>
#include <Math/Vec.h>

using Vec2f = Vec<float, 2>;


template <>
class Vec<float, 2>
{
public:
    inline Vec(const SIMD4F& v)
    {
        SIMD4F_USTORE2(v, this->Data());
    }

public:
    inline Vec()
        : x(0.0f)
        , y(0.0f)
    {

    }

    inline Vec(const Vec<float, 2>& v)
        : x(v.x)
        , y(v.y)
    {

    }

    inline Vec(float vx, float vy)
        : x(vx)
        , y(vy)
    {
        
    }

    inline Vec(float* v)
        : x(v[0])
        , y(v[1])
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

public:
    inline float& operator()(const uint32_t i)
    { 
        ASSERT_MSG(i >= 0 && i < 2, "Over range Vec2.");
        return *(reinterpret_cast<float*>(&x) + i);
    }

    inline const float& operator()(const uint32_t i) const
    { 
        ASSERT_MSG(i >= 0 && i < 2, "Over range Vec2.");
        return *(reinterpret_cast<const float*>(&x) + i);
    }

    inline float& operator[](const uint32_t i)
    {
        ASSERT_MSG(i >= 0 && i < 2, "Over range Vec2.");
        return *(reinterpret_cast<float*>(&x) + i);
    }

    inline const float& operator[](const uint32_t i) const
    {
        ASSERT_MSG(i >= 0 && i < 2, "Over range Vec2.");
        return *(reinterpret_cast<const float*>(&x) + i);
    }

    inline Vec<float, 2> operator-() const
    {
        return Vec<float, 2>(SIMD4F_SUB(SIMD4F_ZERO(), SIMD4F_ULOAD2(this->Data())));
    }

    inline Vec<float, 2> operator*(const Vec<float, 2>& v) const
    {
        return Vec<float, 2>(SIMD4F_MUL(SIMD4F_ULOAD2(this->Data()), SIMD4F_ULOAD2(v.Data())));
    }

    inline Vec<float, 2> operator/(const Vec<float, 2>& v) const
    {
        return Vec<float, 2>(SIMD4F_DIV(SIMD4F_ULOAD2(this->Data()), SIMD4F_ULOAD2(v.Data())));
    }

    inline Vec<float, 2> operator+(const Vec<float, 2>& v) const
    {
        return Vec<float, 2>(SIMD4F_ADD(SIMD4F_ULOAD2(this->Data()), SIMD4F_ULOAD2(v.Data())));
    }

    inline Vec<float, 2> operator-(const Vec<float, 2>& v) const
    {
        return Vec<float, 2>(SIMD4F_SUB(SIMD4F_ULOAD2(this->Data()), SIMD4F_ULOAD2(v.Data())));
    }

    inline Vec<float, 2> operator*(float s) const
    {
        return Vec<float, 2>(SIMD4F_MUL(SIMD4F_ULOAD2(this->Data()), SIMD4F_SET(s)));
    }

    inline Vec<float, 2> operator/(float s) const
    {
        return Vec<float, 2>(SIMD4F_DIV(SIMD4F_ULOAD2(this->Data()), SIMD4F_SET(s)));
    }

    inline Vec<float, 2> operator+(float s) const
    {
        return Vec<float, 2>(SIMD4F_ADD(SIMD4F_ULOAD2(this->Data()), SIMD4F_SET(s)));
    }

    inline Vec<float, 2> operator-(float s) const
    {
        return Vec<float, 2>(SIMD4F_SUB(SIMD4F_ULOAD2(this->Data()), SIMD4F_SET(s)));
    }

    inline Vec<float, 2> operator*=(const Vec<float, 2>& v)
    {
        *this = SIMD4F_MUL(SIMD4F_ULOAD2(this->Data()), SIMD4F_ULOAD2(v.Data()));
        return *this;
    }

    inline Vec<float, 2> operator/=(const Vec<float, 2>& v)
    {
        *this = SIMD4F_DIV(SIMD4F_ULOAD2(this->Data()), SIMD4F_ULOAD2(v.Data()));
        return *this;
    }

    inline Vec<float, 2> operator+=(const Vec<float, 2>& v)
    {
        *this = SIMD4F_ADD(SIMD4F_ULOAD2(this->Data()), SIMD4F_ULOAD2(v.Data()));
        return *this;
    }

    inline Vec<float, 2> operator-=(const Vec<float, 2>& v)
    {
        *this = SIMD4F_SUB(SIMD4F_ULOAD2(this->Data()), SIMD4F_ULOAD2(v.Data()));
        return *this;
    }

    inline Vec<float, 2> operator*=(float s)
    {
        *this = SIMD4F_MUL(SIMD4F_ULOAD2(this->Data()), SIMD4F_SET(s));
        return *this;
    }

    inline Vec<float, 2> operator/=(float s)
    {
        *this = SIMD4F_MUL(SIMD4F_ULOAD2(this->Data()), SIMD4F_SET(s));
        return *this;
    }

    inline Vec<float, 2> operator+=(float s)
    {
        *this = SIMD4F_MUL(SIMD4F_ULOAD2(this->Data()), SIMD4F_SET(s));
        return *this;
    }

    inline Vec<float, 2> operator-=(float s)
    {
        *this = SIMD4F_MUL(SIMD4F_ULOAD2(this->Data()), SIMD4F_SET(s));
        return *this;
    }

    inline bool operator==(const Vec<float, 2>& v) const
    {
        for (int i = 0; i < 2; ++i)
        {
            if ((*this)[i] != v[i])
            {
                return false;
            }
        }
        return true;
    }

    inline bool operator!=(const Vec<float, 2>& v) const
    {
        return !operator==(v);
    }

    inline float GetLengthSquared() const
    {
        return SIMD4F_DOT2_SCALAR(SIMD4F_ULOAD2(this->Data()), SIMD4F_ULOAD2(this->Data()));
    }

    inline float GetLength() const 
    {
        return SIMD4F_GET_X(SIMD4F_LENGTH2(SIMD4F_ULOAD2(this->Data())));
    }

    inline Vec<float, 2> GetNormalized() const
    {
        return Vec<float, 2>(SIMD4F_NORMALIZE2(SIMD4F_ULOAD2(this->Data())));
    }

    inline void Normalized()
    {
        *this = SIMD4F_NORMALIZE2(SIMD4F_ULOAD2(this->Data()));
    }

    static inline Vec2f Cross(const Vec2f& lhs, const Vec2f& rhs)
    {
        return Vec2f();
    }

    static inline float Dot(const Vec2f& lhs, const Vec2f& rhs)
    {
        return SIMD4F_DOT2_SCALAR(SIMD4F_ULOAD2(lhs.Data()), SIMD4F_ULOAD2(rhs.Data()));
    }



public:

    float x;
    float y;

};
