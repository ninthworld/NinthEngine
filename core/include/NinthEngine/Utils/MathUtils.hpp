#pragma once

#include <algorithm>
#include <math.h>
#include "GLM\glm.hpp"
#include "GLM\ext.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "GLM\gtx\extend.hpp"

#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062

#define UMOD(a, b)  (a < 0 ? (((a % b) + b) % b) : (a % b))

namespace NinthEngine {

typedef glm::vec4 Color;

struct Viewport {
	int x, y, width, height;
};

struct AABB {
	glm::vec3 min, max;
	AABB operator-(const glm::vec3 p) { return AABB{ min - p, max - p }; };
	AABB operator-() { return AABB{ -min, -max }; };
	AABB operator+(const glm::vec3 p) { return AABB{ min + p, max + p }; };
};

} // namespace NinthEngine