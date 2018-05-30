#pragma once
#include <Platform/Platform.h>

#ifdef GE_PLATFORM_SSE
#include <Math/Accel/MathAccelSSE.h>
#else
#include <Math/Accel/MathAccelGeneral.h>
#endif

