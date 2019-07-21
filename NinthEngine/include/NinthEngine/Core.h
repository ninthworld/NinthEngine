#pragma once

#ifdef NE_PLATFORM_WIN32
# ifdef NE_BUILD_DLL
#  define NE_API __declspec(dllexport)
# else
#  define NE_API __declspec(dllimport)
# endif
#else
# error Unsupported Platform
#endif

#ifdef NE_ENABLE_ASSERTS
# define NE_ASSERT(x, ...) { if(!(x)) { Log::error("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
# define NE_ASSERT(x, ...)
#endif