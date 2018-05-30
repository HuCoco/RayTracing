// Copyright 2017-2018 Hu Ke. All rights reserved.
#pragma once

#ifndef __GNUC__
#error This file should only be included in g++ compiler
#endif

//! Compiler features
#if defined(__EXCEPTIONS)
#define CRY_COMPILER_EXCEPTIONS 1
#endif
#if defined(__GXX_RTTI)
#define CRY_COMPILER_RTTI 1
#endif

#define GE_ALIGN_HEAD(alignment) __declspec(align(alignment))
#define GE_ALIGN_TAIL(alignment)

#define IF(condition, hint)    if (condition)
#define IF_UNLIKELY(condition) if (condition)
#define IF_LIKELY(condition)   if (condition)

#define NO_INLINE        __declspec(noinline)
#define NO_INLINE_WEAK   __declspec(noinline) inline
#define GE_FORCE_INLINE __forceinline