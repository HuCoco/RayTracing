// Copyright 2017-2018 Hu Ke. All rights reserved.
#pragma once

#ifndef _MSC_VER
#error This file should only be included on MSVC compiler
#endif

//MSVC++ 14.1 _MSC_VER == 1912 (Visual Studio 2017)
//MSVC++ 14.1 _MSC_VER == 1911 (Visual Studio 2017)
//MSVC++ 14.1 _MSC_VER == 1910 (Visual Studio 2017)
//MSVC++ 14.0 _MSC_VER == 1900 (Visual Studio 2015)
//MSVC++ 12.0 _MSC_VER == 1800 (Visual Studio 2013)
//MSVC++ 11.0 _MSC_VER == 1700 (Visual Studio 2012)
//MSVC++ 10.0 _MSC_VER == 1600 (Visual Studio 2010)
//MSVC++ 9.0  _MSC_FULL_VER == 150030729 (Visual Studio 2008, SP1)
//MSVC++ 9.0  _MSC_VER == 1500 (Visual Studio 2008)
//MSVC++ 8.0  _MSC_VER == 1400 (Visual Studio 2005)
//MSVC++ 7.1  _MSC_VER == 1310 (Visual Studio 2003)
//MSVC++ 7.0  _MSC_VER == 1300
//MSVC++ 6.0  _MSC_VER == 1200
//MSVC++ 5.0  _MSC_VER == 1100

//! Compiler version
#define GE_COMPILER_MSVC    1
#define GE_COMPILER_VERSION (_MSC_VER)

#if defined(_CPPUNWIND)
#define GE_COMPILER_EXCEPTIONS 1
#endif
#if defined(_CPPRTTI)
#define GE_COMPILER_RTTI 1
#endif

#define GE_ALIGN_HEAD(alignment) __declspec(align(alignment))
#define GE_ALIGN_TAIL(alignment)

#define IF(condition, hint)    if (condition)
#define IF_UNLIKELY(condition) if (condition)
#define IF_LIKELY(condition)   if (condition)

#define NO_INLINE        __declspec(noinline)
#define NO_INLINE_WEAK   __declspec(noinline) inline
#define GE_FORCE_INLINE __forceinline