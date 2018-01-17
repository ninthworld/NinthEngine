#version 400

layout (points) in;
layout (triangle_strip, max_vertices=180) out;

out vec3 gs_color;
out vec3 gs_position;
out vec3 gs_normal;

layout(std140) uniform Camera {
	mat4 viewMatrix;
	mat4 viewProjMatrix;
	vec4 camPosition;
};

layout(std140) uniform Terrain {
	mat4 worldMatrix;
	float scaleXZ;
	float scaleY;
	float detailRangeNear;
	float detailRangeFar;
	float lodRange[8];
};

layout(std140) uniform Grass {
	vec4 grassPosition;
};

uniform sampler2D heightmap;
uniform sampler2D densitymap;
uniform sampler2D noisemap;

const vec4 vertsLarge[7] = {
	vec4(-0.10, 0.0, 0.0, 1.0),
	vec4(0.10, 0.0, 0.0, 1.0),
	vec4(-0.10, 0.75, -0.25, 1.0),
	vec4(0.10, 0.75, -0.25, 1.0),
	vec4(-0.10, 1.25, -0.5, 1.0),
	vec4(0.10, 1.25, -0.5, 1.0),
	vec4(0.0, 2.0, -1.25, 1.0)
};

const vec3 colorsLarge[7] = {
	vec3(0, 0, 0),
	vec3(0, 0, 0),
	vec3(0.333, 0.333, 0.333),
	vec3(0.333, 0.333, 0.333),
	vec3(0.666, 0.666, 0.666),
	vec3(0.666, 0.666, 0.666),
	vec3(1, 1, 1)
};

float getHeightAt(vec2 pos) {
	return texture(heightmap, (pos + vec2(scaleXZ / 2.0)) / scaleXZ).r * scaleY;
}

float getDensityAt(vec2 pos) {
	return texture(densitymap, (pos + vec2(scaleXZ / 2.0)) / scaleXZ).r;
}

vec3 getNoiseAt(vec2 pos) {
	return texture(noisemap, (pos + vec2(scaleXZ / 2.0)) / scaleXZ).rgb;
}

mat4 translate(vec4 a) {
	return mat4(
		1, 0, 0, 0, 
		0, 1, 0, 0, 
		0, 0, 1, 0, 
		a);
}

mat4 rotateX(float a) {
	return mat4(
		1, 0, 0, 0,
		0, cos(a), -sin(a), 0,
		0, sin(a), cos(a), 0,
		0, 0, 0, 1);
}

mat4 rotateY(float a) {
	return mat4(
		cos(a), 0, sin(a), 0,
		0, 1, 0, 0,
		-sin(a), 0, cos(a), 0,
		0, 0, 0, 1);
}

vec3 getNormal(vec3 a, vec3 b, vec3 c) {
	return normalize(cross(a - b, a - c));
}

void createTriangle(mat4 matrix, vec4 a, vec4 b, vec4 c, vec3 c0, vec3 c1, vec3 c2) {

	gs_normal = getNormal(a.xyz, b.xyz, c.xyz);

	gs_position = a.xyz;
	gs_color = c0;
	gl_Position = matrix * a;
	EmitVertex();

	gs_position = b.xyz;
	gs_color = c1;
	gl_Position = matrix * b;
	EmitVertex();
	
	gs_position = c.xyz;
	gs_color = c2;
	gl_Position = matrix * c;
	EmitVertex();

	EndPrimitive();
}

void createGrassLarge(mat4 matrix) {
	createTriangle(matrix, vertsLarge[0], vertsLarge[1], vertsLarge[2], colorsLarge[0], colorsLarge[1], colorsLarge[2]);
	createTriangle(matrix, vertsLarge[3], vertsLarge[2], vertsLarge[1], colorsLarge[3], colorsLarge[2], colorsLarge[1]);
	createTriangle(matrix, vertsLarge[2], vertsLarge[3], vertsLarge[4], colorsLarge[2], colorsLarge[3], colorsLarge[4]);
	createTriangle(matrix, vertsLarge[5], vertsLarge[4], vertsLarge[3], colorsLarge[5], colorsLarge[4], colorsLarge[3]);
	createTriangle(matrix, vertsLarge[4], vertsLarge[5], vertsLarge[6], colorsLarge[4], colorsLarge[5], colorsLarge[6]);
}

void main() {
	
	float dist = length(vec3(grassPosition.x, getHeightAt(grassPosition.xz), grassPosition.z) - camPosition.xyz);
	dist = clamp((96.0 - dist) / 96.0, 0, 1);
	int num = int(getDensityAt(grassPosition.xz) * dist * 12);
	for(int j = 0; j < num; ++j) {
		vec3 noise = getNoiseAt(grassPosition.xz * num + j);
		float rotX = noise.x * (3.14159 / 4.0);
		float rotY = noise.y * 3.14159 * 2.0;

		vec2 rPos = (noise.xz * noise.xy * 2.0 - 1.0) * 2.0;
			
		vec4 pos = grassPosition + vec4(rPos.x, 0, rPos.y, 0);
		pos += vec4(0, getHeightAt(pos.xz), 0, 0);	

		mat4 matrix = viewProjMatrix * translate(pos) * rotateY(rotY) * rotateX(rotX);

		createGrassLarge(matrix);
	}
}