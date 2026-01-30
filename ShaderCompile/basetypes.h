#pragma once

#include <cstdint>
#ifdef _DEBUG
	#include <cassert>
#endif

#ifdef _DEBUG
	#define Assert assert
#else
	#define Assert(e)
#endif

#if !defined(_WIN32) && !defined(_WIN64)
  #ifndef strcpy_s
    #define strcpy_s(dst, n, src) strncpy(dst, src, n)
  #endif
  #ifndef sprintf_s
    #define sprintf_s snprintf
  #endif
  #define _strupr_s strupr
  #define _strdup strdup
#endif

#if !defined(_MSC_VER)
  #if defined(__clang__)
    #define __assume(x) do { if (!(x)) __builtin_unreachable(); } while (0)
  #elif defined(__GNUC__)
    #define __assume(x) do { if (!(x)) __builtin_unreachable(); } while (0)
  #else
    #define __assume(x) ((void)0)
  #endif
#endif
