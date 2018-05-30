// Copyright 2017-2018 Hu Ke. All rights reserved.
#pragma once
#include <Platform/Platform.h>

template<typename _T, uint32_t d>
class Vec
{
public:
    inline Vec()
    {
        std::memset(m_Data, 0, sizeof(_T) * d);
    }

    inline Vec(const _T* data)
    {
        std::memcpy(m_Data, data, sizeof(_T) * d);
    }

    inline Vec(const Vec<_T, d>& v)
    {
        std::memcpy(m_Data, v.m_Data, sizeof(_T) * d);
    }

    static inline size_t mem_size()
    {
        return sizeof(_T) * d;
    }

    inline _T& operator()(const uint32_t i)
    {
        return m_Data[i];
    }

    inline const _T& operator()(const uint32_t i) const
    {
        return m_Data[i];
    }

    inline _T& operator[](const uint32_t i)
    {
        return m_Data[i];
    }

    inline const _T& operator[](const uint32_t i) const
    {
        return m_Data[i];
    }

    inline _T* data()
    {
        return m_Data;
    }

    inline const _T* data() const
    {
        return m_Data;
    }

    inline Vec<_T, 2> xy()
    {
        return Vec(_T, 2)(m_Data[0], m_Data[1]);
    }

    inline const Vec<_T, 2> xy() const
    {
        return Vec(_T, 2)(m_Data[0], m_Data[1]);
    }

    inline Vec<_T, 3> xyz()
    {
        return Vec(_T, 3)(m_Data[0], m_Data[1], m_Data[2]);
    }

    inline const Vec<_T, 3> xyz() const
    {
        return Vec(_T, 3)(m_Data[0], m_Data[1], m_Data[2]);
    }


private:
    _T m_Data[d];
};






