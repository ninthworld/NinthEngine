#pragma once

namespace NinthEngine {

typedef int INT;
typedef float FLOAT;

struct FLOAT2 {
	FLOAT x, y;
};

struct FLOAT3 {
	FLOAT x, y, z;
};

struct FLOAT4 {
	FLOAT x, y, z, w;
};

struct MATRIX3 {
	FLOAT m00, m01, m02;
	FLOAT m10, m11, m12;
	FLOAT m20, m21, m22;
};

struct MATRIX4 {
	FLOAT m00, m01, m02, m03;
	FLOAT m10, m11, m12, m13;
	FLOAT m20, m21, m22, m23;
	FLOAT m30, m31, m32, m33;
};

} // namespace NinthEngine