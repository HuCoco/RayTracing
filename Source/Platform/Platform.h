#pragma once
#include <Platform/PlatformDefines.h>


#ifdef _WINDOWS_
#error windows.h should not be included prior to platform.h
#endif


// Safe memory
#define SAFE_DELETE(p)        { if (p) { delete (p);          (p) = NULL; } }
#define SAFE_DELETE_ARRAY(p)  { if (p) { delete[] (p);        (p) = NULL; } }


#ifndef CHECK_REFERENCE_COUNTS   
#define CHECK_REFERENCE_COUNTS 0 
#endif

#if CHECK_REFERENCE_COUNTS
#define CHECK_REFCOUNT_CRASH(x) { if (!(x)) *((int*)0) = 0; }
#else
#define CHECK_REFCOUNT_CRASH(x)
#endif

// Include standard CRT headers
#include <cstdint>
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>

#include <memory>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <stack>
#include <algorithm>
#include <functional>
#include <iterator>
#include <limits>
#include <cmath>
#include <assert.h>
#ifdef _WINDOWS_
#error windows.h should not be included through any headers within platform.h
#endif

#ifdef GE_PLATFORM_WINDOWS
#include <Windows.h>
#endif


#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#ifdef far
#undef far
#endif

#ifdef near
#undef near
#endif

#define ASSERT_MSG(exp,msg) assert(exp)