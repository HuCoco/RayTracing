#pragma once
#include <Platform/Platform.h>
#include <cmath>

const uint32_t LeadingBitTable[] =
{
    0,  9,  1, 10, 13, 21,  2, 29,
    11, 14, 16, 18, 22, 25,  3, 30,
    8, 12, 20, 28, 15, 17, 24,  7,
    19, 27, 23,  6, 26,  5,  4, 31
};

const uint32_t TrailingBitTable[] =
{
    0,  1, 28,  2, 29, 14, 24,  3,
    30, 22, 20, 15, 25, 17,  4,  8,
    31, 27, 13, 23, 21, 19, 16,  7,
    26, 12, 18,  6, 11,  5, 10,  9
};


const float PI = 3.14159265358979323846264338327950288419716939937510f;
const float PI_2 = 3.14159265358979323846264338327950288419716939937510f * 2.0f;
const float HALF_PI = 3.14159265358979323846264338327950288419716939937510f / 2.0f;
#define DEG2RAD(a) ((a) * (PI / 180.0f))
#define RAD2DEG(a) ((a) * (180.0f / PI))
const float ln2 = 0.69314718055994530941723212145818f;
const float sqrt2 = 1.4142135623730950488016887242097f;
const float sqrt3 = 1.7320508075688772935274463415059f;

#define GET_LOW(v) (uint32_t)((v) & 0x00000000ffffffffULL)
#define GET_HIGH(v) (uint32_t)(((v) >> 32) & 0x00000000ffffffffULL)

#define BIT_1_GET_EVEN_POSITION		0xAAAAAAAAu
#define BIT_1_GET_ODD_POSITION		0x55555555u
#define BIT_2_GET_EVEN_POSITION		0x33333333u
#define BIT_2_GET_ODD_POSITION		0xCCCCCCCCu
#define BIT_4_GET_EVEN_POSITION		0x0F0F0F0Fu
#define BIT_4_GET_ODD_POSITION		0xF0F0F0F0u
#define BIT_8_GET_EVEN_POSITION		0x00FF00FFu
#define BIT_8_GET_ODD_POSITION		0xFF00FF00u
#define BIT_16_GET_EVEN_POSITION	0x0000FFFFu
#define BIT_16_GET_ODD_POSITION		0xFFFF0000u

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif


#define LEADING_BIT_NUMBER			0x07C4ACDDu



namespace Math
{
    template<typename _T>
    static inline _T min(_T a, _T b)
    {
        return b < a ? b : a;
    }

    template<typename _T>
    static inline _T max(_T a, _T b)
    {
        return a < b ? b : a;
    }

    template<typename _T>
    static inline _T convert(_T value)
    {
        return static_cast<_T>(value);
    }

    using std::abs;
    using std::floor;
    using std::ceil;
    using std::trunc;

    template<typename _T> 
    static inline _T clamp(_T val, _T lo, _T hi)
    {
        return min(max(val, lo), hi);
    }

    template<typename _T>
    static inline _T saturate(_T val)
    { 
        return clamp(val, convert<T>(0.0f), convert<T>(1.0f));
    }

    using std::sin;
    using std::cos;
    using std::tan;

    template<typename T> 
    static inline T asin(T op)
    {
        return std::asin(clamp(op, convert<T>(-1), convert<T>(1)));
    }

    template<typename T> 
    static inline T acos(T op)
    {
        return std::acos(clamp(op, convert<T>(-1), convert<T>(1)));
    }

    using std::atan;
    using std::atan2;

    template<typename _T> 
    static inline void sincos(_T angle, _T* pSin, _T* pCos)
    { 
        *pSin = sin(angle); 
        *pCos = cos(angle);
    }

    using std::exp;
    using std::exp;
    using std::log;
    using std::pow;
    using std::sqrt;

    template<typename _T>
    static inline _T rsqrt(_T op)
    {
        return convert<_T>(1) / std::sqrt(op);
    }

    template<>
    static inline float rsqrt(float op)
    {
        uint64_t i;
        float x2, y;
        const float threehalfs = 1.5f;
        x2 = op * 0.5F;
        y = op;
        i = *reinterpret_cast<int32_t*>(&y);
        i = 0x5F375A86u - (i >> 1);
        y = *reinterpret_cast<float*>(&i);
        y = y * (threehalfs - (x2 * y * y));
        return y;
    }

    template<typename T> 
    static inline T Align(T data, size_t align)
    {
        ASSERT_MSG((align & (align - 1)) == 0, "Error alignment value");
        size_t size = ((size_t)data + (align - 1)) & ~(align - 1);
        return T(size);
    }

    template<typename T>
    static inline bool IsAligned(T data, size_t align)
    {
        ASSERT_MSG((align & (align - 1)) == 0, "Error alignment value");
        return (size_t(data) & (align - 1)) == 0;
    }

    static inline uint32_t GetLeadingBit32(uint32_t value)
    {
        value |= value >> 1;
        value |= value >> 2;
        value |= value >> 4;
        value |= value >> 8;
        value |= value >> 16;
        uint32_t key = (value * LEADING_BIT_NUMBER) >> 27;
        return LeadingBitTable[key];
    }

    static inline uint32_t GetLeadingBit64(uint64_t value)
    {
        uint32_t v1 = GET_HIGH(value);
        if (v1 != 0)
        {
            return GetLeadingBit32(v1) + 32;
        }

        uint32_t v0 = GET_LOW(value);
        return GetLeadingBit32(v0);
    }

    static inline uint32_t RoundUpToPowerOfTwo(uint32_t value)
    {
        if (value > 0)
        {
            uint32_t leading = GetLeadingBit32(value);
            uint32_t mask = (1 << leading);
            if ((value & ~mask) == 0)
            {
                return static_cast<uint32_t>(value);
            }
            else
            {
                return (static_cast<uint32_t>(mask) << 1);
            }
        }
        else
        {
            return 1;
        }
    }

    static inline uint32_t RoundDownToPowerOfTwo(uint32_t value)
    {
        if (value > 0)
        {
            uint32_t leading = GetLeadingBit32(value);
            uint32_t mask = (1 << leading);
            return mask;
        }
        else
        {
            return 0;
        }
    }

    static inline uint32_t Log2(uint32_t powerOfTwo)
    {
        uint32_t log2 = (powerOfTwo & BIT_1_GET_EVEN_POSITION) != 0;
        log2 |= ((powerOfTwo & BIT_16_GET_ODD_POSITION) != 0) << 4;
        log2 |= ((powerOfTwo & BIT_8_GET_ODD_POSITION) != 0) << 3;
        log2 |= ((powerOfTwo & BIT_4_GET_ODD_POSITION) != 0) << 2;
        log2 |= ((powerOfTwo & BIT_2_GET_ODD_POSITION) != 0) << 1;
        return log2;
    }


};