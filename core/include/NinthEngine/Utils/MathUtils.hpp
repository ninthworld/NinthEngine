#pragma once

#include <math.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtx\extend.hpp>

#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062

namespace NinthEngine {

typedef glm::vec4 Color;

struct Viewport {
	float x, y, width, height;
};

} // namespace NinthEngine