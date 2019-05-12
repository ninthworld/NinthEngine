#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>

#include "NinthEngine/Log/Log.h"

#ifdef NE_PLATFORM_WIN32
# include <Windows.h>
# include <windowsx.h>
#endif

struct Color {
	float r, g, b, a;
	Color(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f)
		: r(r), g(g), b(b), a(a) {}
};