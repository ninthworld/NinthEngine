#version 400

#define PI	3.14159265359

layout(location=0) in vec3 in_position;
layout(location=1) in vec3 in_normal;
layout(location=2) in vec2 in_texCoord;

out vec3 vs_normal;
out vec2 vs_texCoord;
out float vs_alpha;

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

layout(std140) uniform Node {
	mat4 localMatrix;
	vec4 neighbors;
	vec2 location;
	vec2 index;
	float size;
	int lod;
	vec2 _nPadding;
};

layout(std140) uniform Grass {
	vec4 timeStep;
};

uniform sampler2D heightmap;
uniform sampler2D normalmap;
uniform sampler2D alphamap;

mat4 makeTranslation(vec4 a);
mat4 makeScale(vec4 a);
mat4 makeRotationX(float a);
mat4 makeRotationY(float a);
float snoise(vec2 v);

#define OFFSET		0.5
#define MIN_SCALE	0.75
#define MAX_SCALE	1.0

#define WIND_SPEED	2.0
#define WIND_DISP	0.5

#define VIEW_DIST	120
#define FADE_DIST	50

void main() {

	// Instancing
	int sideCount = 10 + (8 - lod) * 4;
	int instance = gl_InstanceID;
	vec2 instancePos = vec2(floor(instance / sideCount), instance % sideCount);
	
	// Positioning
	vec2 localPos = location + size * instancePos / sideCount;
	
	// Transformations
	// Add random offset
	vec2 randomOffset = ((vec2(snoise(scaleXZ * localPos + 0), snoise(scaleXZ * localPos + 1)) * 2.0 - 1.0) * OFFSET) / scaleXZ;
	localPos += randomOffset;
	
	float randomAngle = snoise(scaleXZ * localPos + 2) * (PI * 2.0);
	float randomScale = mix(MIN_SCALE, MAX_SCALE, snoise(scaleXZ * localPos + 3));

	vec4 vertexPos = makeRotationY(randomAngle) * makeScale(vec4(vec3(randomScale), 1.0)) * vec4(in_position, 1.0);

	// Grass Wave
	if (in_position.y > 0) { // Only the top vertices
		float wind = sin((localPos.x + localPos.y) * scaleXZ / 4 + WIND_SPEED * timeStep.x * PI);
		vertexPos.x += wind * cos(WIND_SPEED * timeStep.x * PI) * WIND_DISP;
		vertexPos.z += wind * sin(WIND_SPEED * timeStep.x * PI) * WIND_DISP;
	}

	// Final calculations
	vec2 worldPosXZ = (localPos - 0.5) * scaleXZ;
	float height = texture(heightmap, localPos).r * scaleY;
	vec3 worldPos = vec3(worldPosXZ.x, height, worldPosXZ.y);

	// Send to Fragment Shader
	vs_alpha = (1.0 - clamp((length(camPosition.xyz - worldPos) - VIEW_DIST) / FADE_DIST, 0, 1));
	if (texture(alphamap, localPos).x < 0.6) vs_alpha = 0;
	vs_texCoord = in_texCoord * 0.5 + vec2(0, 0.001);
	vs_normal = texture(normalmap, localPos).xyz;
	gl_Position = viewProjMatrix * (vertexPos + vec4(worldPos, 0));
}

mat4 makeTranslation(vec4 a) {
	return mat4(
		1, 0, 0, 0, 
		0, 1, 0, 0, 
		0, 0, 1, 0, 
		a);
}

mat4 makeScale(vec4 a) {
	return mat4(
		a.x, 0, 0, 0, 
		0, a.y, 0, 0, 
		0, 0, a.z, 0, 
		0, 0, 0, a.w);
}

mat4 makeRotationX(float a) {
	return mat4(
		1, 0, 0, 0,
		0, cos(a), -sin(a), 0,
		0, sin(a), cos(a), 0,
		0, 0, 0, 1);
}

mat4 makeRotationY(float a) {
	return mat4(
		cos(a), 0, sin(a), 0,
		0, 1, 0, 0,
		-sin(a), 0, cos(a), 0,
		0, 0, 0, 1);
}

//
// Description : Array and textureless GLSL 2D simplex noise function.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : stegu
//     Lastmod : 20110822 (ijm)
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//               Distributed under the MIT License. See LICENSE file.
//               https://github.com/ashima/webgl-noise
//               https://github.com/stegu/webgl-noise
// 

vec3 mod289(vec3 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec2 mod289(vec2 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec3 permute(vec3 x) {
  return mod289(((x*34.0)+1.0)*x);
}

float snoise(vec2 v)
  {
  const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
                      0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)
                     -0.577350269189626,  // -1.0 + 2.0 * C.x
                      0.024390243902439); // 1.0 / 41.0
// First corner
  vec2 i  = floor(v + dot(v, C.yy) );
  vec2 x0 = v -   i + dot(i, C.xx);

// Other corners
  vec2 i1;
  //i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0
  //i1.y = 1.0 - i1.x;
  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
  // x0 = x0 - 0.0 + 0.0 * C.xx ;
  // x1 = x0 - i1 + 1.0 * C.xx ;
  // x2 = x0 - 1.0 + 2.0 * C.xx ;
  vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;

// Permutations
  i = mod289(i); // Avoid truncation effects in permutation
  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
		+ i.x + vec3(0.0, i1.x, 1.0 ));

  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
  m = m*m ;
  m = m*m ;

// Gradients: 41 points uniformly over a line, mapped onto a diamond.
// The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)

  vec3 x = 2.0 * fract(p * C.www) - 1.0;
  vec3 h = abs(x) - 0.5;
  vec3 ox = floor(x + 0.5);
  vec3 a0 = x - ox;

// Normalise gradients implicitly by scaling m
// Approximation of: m *= inversesqrt( a0*a0 + h*h );
  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );

// Compute final noise value at P
  vec3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
  return 130.0 * dot(m, g);
}