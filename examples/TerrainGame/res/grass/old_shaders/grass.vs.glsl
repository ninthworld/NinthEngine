#version 400

layout(location=0) in vec3 in_position;
layout(location=0) in vec3 in_normal;
layout(location=0) in vec3 in_color;

out vec3 vs_color;
out vec3 vs_normal;
out vec3 vs_position;

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

void main() {
	
	int index = gl_InstanceID;

	vec4 gPos = vec4(floor(floor(index / 100.0)), 0.0, float(index % 100), 1.0);

	vec3 noise = getNoiseAt(gPos.xz * 4.0);
	float rotX = noise.x * (3.14159 / 4.0);
	float rotY = noise.y * 3.14159 * 2.0;

	vec2 rPos = (noise.xz * noise.xy * 2.0 - 1.0) * 2.0;
			
	vec4 pos = gPos + vec4(rPos.x, 0, rPos.y, 0);
	pos += vec4(0, getHeightAt(pos.xz), 0, 0);	
	
	vs_position = (translate(pos) * rotateY(rotY) * rotateX(rotX) * vec4(in_position, 1.0)).xyz;
	vs_normal = in_normal;
	vs_color = in_color;
	gl_Position = viewProjMatrix * vec4(vs_position, 1.0);
}