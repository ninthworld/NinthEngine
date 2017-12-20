#pragma once

#define GLEW_STATIC
#include <GL\glew.h>

#ifdef _WIN32
#include <GL\wglew.h>
#endif

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32s.lib")