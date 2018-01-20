#version 400

in vec2 gs_texCoord;
in vec3 gs_position;
in vec3 gs_tangent;

layout(location=0) out vec4 ps_color;
layout(location=1) out vec4 ps_normal;

layout(std140) uniform Camera {
	mat4 viewMatrix;
	mat4 viewProjMatrix;
	vec4 camPosition;
};

layout(std140) uniform Terrain {
	vec4 terrainScale;
};

uniform sampler2D normalmap;
uniform sampler2D fractal01_normalmap;
uniform sampler2D fractal02_normalmap;
uniform sampler2D blobmap;

// Grass
uniform sampler2D material0_color;
uniform sampler2D material0_normal;

// Dirt 1
uniform sampler2D material1_color;
uniform sampler2D material1_normal;

// Dirt 2
uniform sampler2D material2_color;
uniform sampler2D material2_normal;

// Stone 1
uniform sampler2D material3_color;
uniform sampler2D material3_normal;

// Stone 2
uniform sampler2D material4_color;
uniform sampler2D material4_normal;

float getMap(float slope, vec4 phase) {
	if (slope >= phase.x && slope < phase.y) {
		float d = phase.y - phase.x;
		if (d == 0) return 1.0;
		return (slope - phase.x) / d;
	}
	else if (slope >= phase.y && slope < phase.z) {
		return 1.0;
	}
	else if (slope >= phase.z && slope < phase.w) {
		float d = phase.w - phase.z;
		if (d == 0) return 1.0;
		return 1.0 - (slope - phase.z) / d;
	}
	else {
		return 0;
	}
}

void main() {
	
	vec2 mapPosition = (gs_position.xz + terrainScale.x / 2.0) / terrainScale.x;

	vec3 normal = texture(normalmap, mapPosition).rgb;
	normal += texture(fractal01_normalmap, mapPosition).rgb / 32.0;
	normal += texture(fractal01_normalmap, mapPosition).rgb / 64.0;
	normal = normalize(normal * 2.0 - 1.0);
	
	vec3 grassColor = texture(material0_color, gs_texCoord).rgb;
	vec3 grassNormal = texture(material0_normal, gs_texCoord).rgb;

	vec3 dirt1Color = texture(material1_color, gs_texCoord).rgb;
	vec3 dirt1Normal = texture(material1_normal, gs_texCoord).rgb;

	vec3 dirt2Color = texture(material2_color, gs_texCoord).rgb;
	vec3 dirt2Normal = texture(material2_normal, gs_texCoord).rgb;

	vec3 stone1Color = texture(material3_color, gs_texCoord / 2).rgb;
	vec3 stone1Normal = texture(material3_normal, gs_texCoord / 2).rgb;
	
	vec3 stone2Color = texture(material4_color, gs_texCoord / 2).rgb;
	vec3 stone2Normal = texture(material4_normal, gs_texCoord / 2).rgb;
	
	float blob = texture(blobmap, mapPosition).r;
	
	vec3 dirtColor = mix(dirt1Color, dirt2Color, blob);
	vec3 dirtNormal = mix(dirt1Normal, dirt2Normal, blob);

	vec3 stoneColor = mix(stone1Color, stone2Color, blob) * 0.6;
	vec3 stoneNormal = mix(stone1Normal, stone2Normal, blob);
	
	float slope = 1.0 - normal.z;
	
	vec4 grassPhase = vec4(0.0, 0.0, 0.2, 0.25);
	vec4 dirtPhase = vec4(0.2, 0.25, 0.3, 0.35);
	vec4 stonePhase = vec4(0.3, 0.35, 1.0, 1.0);

	float grassMap = getMap(slope, grassPhase);
	float dirtMap = getMap(slope, dirtPhase);
	float stoneMap = getMap(slope, stonePhase);

	float dist = length(camPosition.xyz - gs_position);
	if (dist < 400) {
		float attenuation = clamp(-dist / 400 + 1, 0, 1);

		vec3 bitangent = normalize(cross(gs_tangent, normal));
		mat3 TBN = mat3(gs_tangent, bitangent, normal);

		vec3 bumpNormal;
		bumpNormal += (grassNormal * 2.0 - 1.0) * grassMap;
		bumpNormal += (dirtNormal * 2.0 - 1.0) * dirtMap;
		bumpNormal += (stoneNormal * 2.0 - 1.0) * stoneMap;
		bumpNormal = normalize(bumpNormal);

		bumpNormal.xy *= attenuation;

		normal = normalize(TBN * bumpNormal);
	}

	ps_normal = vec4((normal + 1.0) / 2.0, 1.0);
	ps_color = vec4(
		grassColor * grassMap +
		dirtColor * dirtMap +
		stoneColor * stoneMap, 1.0);
}