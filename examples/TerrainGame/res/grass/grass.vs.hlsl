
#define PI	3.14159265359

struct VertexIn {
	float3 position : POSITION;
	float3 normal	: NORMAL;
	float2 texCoord : TEXCOORD;
	uint instanceID : SV_InstanceID;
};

struct VertexOut {
	float3 normal	: NORMAL;
	float2 texCoord : TEXCOORD;
	float alpha	: BLEND;
	float4 svPosition : SV_POSITION;
};

cbuffer Camera : register(b0) {
	float4x4 viewMatrix;
	float4x4 viewProjMatrix;
	float4 camPosition;
};

cbuffer Terrain : register(b1) {
	float4x4 worldMatrix;
	float scaleXZ;
	float scaleY;
	float detailRangeNear;
	float detailRangeFar;
	float lodRange[8];
};

cbuffer Node : register(b2) {
	float4x4 localMatrix;
	float4 neighbors;
	float2 location;
	float2 index;
	float size;
	int lod;
	float2 _nPadding;
};

cbuffer Grass : register(b3) {
	float4 timeStep;
};

SamplerState texSampler : register(s0);

Texture2D heightmap : register(t2);
Texture2D normalmap : register(t3);
Texture2D alphamap : register(t4);

float4x4 makeTranslation(float4 a);
float4x4 makeScale(float4 a);
float4x4 makeRotationX(float a);
float4x4 makeRotationY(float a);
float snoise(float2 v);

#define OFFSET		0.5
#define MIN_SCALE	0.75
#define MAX_SCALE	1.0

#define WIND_SPEED	2.0
#define WIND_DISP	0.5

#define VIEW_DIST	120
#define FADE_DIST	50

VertexOut main(VertexIn IN) {
	VertexOut OUT;
	
	// Instancing
	int sideCount = 10 + (8 - lod) * 4;
	float2 instancePos = float2(floor(IN.instanceID / sideCount), IN.instanceID % sideCount);
	
	// Positioning
	float2 localPos = location + size * instancePos / sideCount;
	
	// Transformations
	// Add random offset
	float2 randomOffset = ((float2(snoise(scaleXZ * localPos + 0), snoise(scaleXZ * localPos + 1)) * 2.0 - 1.0) * OFFSET) / scaleXZ;
	localPos += randomOffset;
	
	float randomAngle = snoise(scaleXZ * localPos + 2) * (PI * 2.0);
	float randomScale = lerp(MIN_SCALE, MAX_SCALE, snoise(scaleXZ * localPos + 3));

	float4 vertexPos = mul(makeRotationY(randomAngle), mul(makeScale(float4(randomScale.xxx, 1.0)), float4(IN.position, 1.0)));

	// Grass Wave
	if (IN.position.y > 0) { // Only the top vertices
		float wind = sin((localPos.x + localPos.y) * scaleXZ / 4 + WIND_SPEED * timeStep.x * PI);
		vertexPos.x += wind * cos(WIND_SPEED * timeStep.x * PI) * WIND_DISP;
		vertexPos.z += wind * sin(WIND_SPEED * timeStep.x * PI) * WIND_DISP;
	}

	// Final calculations
	float2 worldPosXZ = (localPos - 0.5) * scaleXZ;
	float height = heightmap.SampleLevel(texSampler, localPos, 0).r * scaleY;
	float3 worldPos = float3(worldPosXZ.x, height, worldPosXZ.y);

	// Send to Fragment Shader
	OUT.alpha = (1.0 - clamp((length(camPosition.xyz - worldPos) - VIEW_DIST) / FADE_DIST, 0, 1));
	if (alphamap.SampleLevel(texSampler, localPos, 0).x < 0.6) OUT.alpha = 0;
	OUT.texCoord = IN.texCoord * 0.5 + float2(0, 0.001);
	OUT.normal = normalmap.SampleLevel(texSampler, localPos, 0).xyz;
	OUT.svPosition = mul(viewProjMatrix, vertexPos + float4(worldPos, 0));

	return OUT;
}

float4x4 makeTranslation(float4 a) {
	return float4x4(
		1, 0, 0, 0, 
		0, 1, 0, 0, 
		0, 0, 1, 0, 
		a);
}

float4x4 makeScale(float4 a) {
	return float4x4(
		a.x, 0, 0, 0, 
		0, a.y, 0, 0, 
		0, 0, a.z, 0, 
		0, 0, 0, a.w);
}

float4x4 makeRotationX(float a) {
	return float4x4(
		1, 0, 0, 0,
		0, cos(a), -sin(a), 0,
		0, sin(a), cos(a), 0,
		0, 0, 0, 1);
}

float4x4 makeRotationY(float a) {
	return float4x4(
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

//// Translated from GLSL to HLSL by Chris Swenson

float3 mod289(float3 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

float2 mod289(float2 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

float3 permute(float3 x) {
  return mod289(((x*34.0)+1.0)*x);
}

float snoise(float2 v)
  {
  const float4 C = float4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
                      0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)
                     -0.577350269189626,  // -1.0 + 2.0 * C.x
                      0.024390243902439); // 1.0 / 41.0
// First corner
  float2 i  = floor(v + dot(v, C.yy) );
  float2 x0 = v -   i + dot(i, C.xx);

// Other corners
  float2 i1;
  //i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0
  //i1.y = 1.0 - i1.x;
  i1 = (x0.x > x0.y) ? float2(1.0, 0.0) : float2(0.0, 1.0);
  // x0 = x0 - 0.0 + 0.0 * C.xx ;
  // x1 = x0 - i1 + 1.0 * C.xx ;
  // x2 = x0 - 1.0 + 2.0 * C.xx ;
  float4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;

// Permutations
  i = mod289(i); // Avoid truncation effects in permutation
  float3 p = permute( permute( i.y + float3(0.0, i1.y, 1.0 ))
		+ i.x + float3(0.0, i1.x, 1.0 ));

  float3 m = max(0.5 - float3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
  m = m*m ;
  m = m*m ;

// Gradients: 41 points uniformly over a line, mapped onto a diamond.
// The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)

  float3 x = 2.0 * frac(p * C.www) - 1.0;
  float3 h = abs(x) - 0.5;
  float3 ox = floor(x + 0.5);
  float3 a0 = x - ox;

// Normalise gradients implicitly by scaling m
// Approximation of: m *= inversesqrt( a0*a0 + h*h );
  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );

// Compute final noise value at P
  float3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
  return 130.0 * dot(m, g);
}