#pragma once

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

#include "Log/Log.h"

#ifdef NE_PLATFORM_WIN32
# include <Windows.h>
# include <windowsx.h>
#endif

namespace NinthEngine {

	struct Color {
		float r, g, b, a;
		Color(float r, float g, float b, float a = 1.0f)
			: r(r), g(g), b(b), a(a) {}
		Color() : Color(0.0f, 0.0f, 0.0f) {}
	};

	enum class PrimitiveType {
		None = 0,
		Points,
		Lines, LineStrip,
		Triangles, TriangleStrip,
		Patches
	};
}