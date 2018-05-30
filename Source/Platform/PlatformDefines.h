// Copyright 2017-2018 Hu Ke. All rights reserved.
#pragma once

#define GE_PLATFORM 1

#if defined(__x86_64__) || defined(_M_X64)
#define GE_PLATFORM_X64         1
#define GE_PLATFORM_64BIT       1
#define GE_PLATFORM_SSE         1
#elif defined(__i386) || defined(_M_IX86)
#define GE_PLATFORM_X86         1
#define GE_PLATFORM_32BIT       1
#elif defined(__arm__)
#define GE_PLATFORM_ARM         1
#define GE_PLATFORM_32BIT       1
#if defined(__ARM_NEON__)
#define GE_PLATFORM_NEON        1
#endif
#else
#define GE_PLATFORM_UNKNOWN_CPU 1
#endif

#if defined(_WIN32)

    #define GE_PLATFORM_DESKTOP 1
    #define GE_PLATFORM_WINDOWS 1
    #define GE_PLATFORM_WINAPI  1

    #if defined(_WIN64)
        #if !GE_PLATFORM_X64
            #error Unsupported Windows 64 CPU (the only supported is x86-64).
        #endif
    #else
        #if !GE_PLATFORM_X86
            #error Unsupported Windows 32 CPU (the only supported is x86).
        #endif
    #endif

#endif


#if defined(GE_PLATFORM_X64) && GE_PLATFORM_X64 != 1
#error Wrong value of CRY_PLATFORM_X64.
#endif

#if defined(GE_PLATFORM_X86) && GE_PLATFORM_X86 != 1
#error Wrong value of CRY_PLATFORM_X86.
#endif

#if defined(__clang__)
#include <Platform/ClangDefines.h>
#elif defined(__GNUC__)
#include <Platform/GCCDefines.h>
#elif defined(_MSC_VER)
#include <Platform/MSVCDefines.h>
#else
#error Unsupported compiler was used.
#endif
