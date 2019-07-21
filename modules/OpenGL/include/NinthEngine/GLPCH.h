#pragma once

#define GLEW_STATIC
# include <GL/glew.h>
#ifdef NE_PLATFORM_WIN32
# include <GL/wglew.h>
#endif

#include <iostream>
#include <fstream>
#include <streambuf>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <chrono>

#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <NinthEngine/Log.h>

#ifdef NE_PLATFORM_WIN32
# include <Windows.h>
# include <windowsx.h>
#endif