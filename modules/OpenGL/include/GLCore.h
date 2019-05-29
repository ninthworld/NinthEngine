#pragma once

#ifdef NE_PLATFORM_WIN32
# ifdef NE_BUILD_DLL
#  define NE_GL_API __declspec(dllexport)
# else
#  define NE_GL_API __declspec(dllimport)
# endif
#else
# error Unsupported Platform
#endif