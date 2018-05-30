// Copyright 2017-2018 Hu Ke. All rights reserved.
#pragma once

#ifndef __clang__
#error This file should only be included on Clang compiler
#endif

#define CRY_COMPILER_CLANG   1
#define CRY_COMPILER_VERSION ((__clang_major__ * 100) + (__clang_minor__))

//! Compiler features
#if __has_feature(cxx_exceptions)
#define CRY_COMPILER_EXCEPTIONS 1
#endif
#if __has_feature(cxx_rtti)
#define CRY_COMPILER_RTTI 1
#endif

#define GE_ALIGN_HEAD(alignment) 
#define GE_ALIGN_TAIL(alignment) __attribute__((aligned(alignment)))

#define IF(condition, hint)    if (__builtin_expect(!!(condition), hint))
#define IF_UNLIKELY(condition) if (__builtin_expect(!!(condition), 0))
#define IF_LIKELY(condition)   if (__builtin_expect(!!(condition), 1))

#define NO_INLINE        __attribute__ ((noinline))
#define NO_INLINE_WEAK   __attribute__ ((noinline)) __attribute__((weak))
#define GE_FORCE_INLINE __attribute__((always_inline)) inline