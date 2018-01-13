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

struct AABB {
	glm::vec3 min, max;
	AABB operator-(const glm::vec3 p) { return AABB{ min - p, max - p }; };
	AABB operator-() { return AABB{ -min, -max }; };
	AABB operator+(const glm::vec3 p) { return AABB{ min + p, max + p }; };
};

struct Plane {
	glm::vec4 points[4];
	glm::vec4 normal;
};

static Plane createPlane(glm::vec3 point0, glm::vec3 point1, glm::vec3 point2, glm::vec3 point3) {
	return Plane{
		glm::vec4(point0, 1.0), glm::vec4(point1, 1.0), glm::vec4(point2, 1.0), glm::vec4(point3, 1.0),
		glm::vec4(glm::normalize(glm::cross(point1 - point0, point3 - point0)), 1.0)
	};
};

static bool isInFrustum(const AABB box, const glm::vec4 frustum[6]) {
	// Code from http://www.iquilezles.org/www/articles/frustumcorrect/frustumcorrect.htm
	// inigo quilez 1994-2017
	for (int i = 0; i<6; i++) {
		int out = 0;
		out += ((glm::dot(frustum[i], glm::vec4(box.min.x, box.min.y, box.min.z, 1.0f)) < 0.0) ? 1 : 0);
		out += ((glm::dot(frustum[i], glm::vec4(box.max.x, box.min.y, box.min.z, 1.0f)) < 0.0) ? 1 : 0);
		out += ((glm::dot(frustum[i], glm::vec4(box.min.x, box.max.y, box.min.z, 1.0f)) < 0.0) ? 1 : 0);
		out += ((glm::dot(frustum[i], glm::vec4(box.max.x, box.max.y, box.min.z, 1.0f)) < 0.0) ? 1 : 0);
		out += ((glm::dot(frustum[i], glm::vec4(box.min.x, box.min.y, box.max.z, 1.0f)) < 0.0) ? 1 : 0);
		out += ((glm::dot(frustum[i], glm::vec4(box.max.x, box.min.y, box.max.z, 1.0f)) < 0.0) ? 1 : 0);
		out += ((glm::dot(frustum[i], glm::vec4(box.min.x, box.max.y, box.max.z, 1.0f)) < 0.0) ? 1 : 0);
		out += ((glm::dot(frustum[i], glm::vec4(box.max.x, box.max.y, box.max.z, 1.0f)) < 0.0) ? 1 : 0);
		if (out == 8) return false;
	}
	return true;
};

} // namespace NinthEngine