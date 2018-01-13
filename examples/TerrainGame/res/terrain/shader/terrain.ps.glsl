#version 400

in vec2 gs_texCoord;
in vec4 gs_viewSpacePos;
in vec3 gs_position;
in vec3 gs_tangent;

layout(location=0) out vec4 ps_color;
layout(location=1) out vec4 ps_normal;
layout(location=2) out vec4 ps_position;

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

uniform sampler2D normalmap;

uniform sampler2D material0Dif;
uniform sampler2D material0Norm;
uniform sampler2D material0Alpha;

uniform sampler2D material1Dif;
uniform sampler2D material1Norm;
uniform sampler2D material1Alpha;

uniform sampler2D material2Dif;
uniform sampler2D material2Norm;
uniform sampler2D material2Alpha;

uniform sampler2D material3Dif;
uniform sampler2D material3Norm;
uniform sampler2D material3Alpha;

void main() {
	
	float dist = length(camPosition.xyz - gs_position);
	float height = gs_position.y;

	vec2 mapCoords = (gs_position.xz + scaleXZ / 2.0) / scaleXZ;

	vec3 normal = normalize(texture(normalmap, mapCoords).rgb * 2.0 - 1.0);

	float material0Scale = 4.0;
	float material1Scale = 4.0;
	float material2Scale = 10.0;
	float material3Scale = 10.0;

	float alpha0 = texture(material0Alpha, mapCoords).r;
	float alpha1 = texture(material1Alpha, mapCoords).r;
	float alpha2 = texture(material2Alpha, mapCoords).r;
	float alpha3 = texture(material3Alpha, mapCoords).r;
	
	vec3 color0 = texture(material0Dif, gs_texCoord / material0Scale).rgb;
	vec3 color1 = texture(material1Dif, gs_texCoord / material1Scale).rgb;
	vec3 color2 = texture(material2Dif, gs_texCoord / material2Scale).rgb;
	vec3 color3 = texture(material3Dif, gs_texCoord / material3Scale).rgb;

	if (dist < detailRangeNear) {
		float attenuation = clamp(-dist / detailRangeNear + 1, 0, 1);

		vec3 bitangent = normalize(cross(gs_tangent, normal));
		mat3 TBN = mat3(gs_tangent, bitangent, normal);

		vec3 bumpNormal = vec3(0);
		bumpNormal += (texture(material0Norm, gs_texCoord / material0Scale).rgb * 2.0 - 1.0) * alpha0;
		bumpNormal += (texture(material1Norm, gs_texCoord / material1Scale).rgb * 2.0 - 1.0) * alpha1;
		bumpNormal += (texture(material2Norm, gs_texCoord / material2Scale).rgb * 2.0 - 1.0) * alpha2;
		bumpNormal += (texture(material3Norm, gs_texCoord / material3Scale).rgb * 2.0 - 1.0) * alpha3;
		bumpNormal = normalize(bumpNormal);

		bumpNormal.xy *= attenuation;

		normal = normalize(TBN * bumpNormal);
	}

	ps_position = vec4(gs_position, 1.0);

	ps_normal = vec4((normal + 1.0) / 2.0, 1.0);
	
	ps_color = vec4(
		color0 * alpha0 +
		color1 * alpha1 +
		color2 * alpha2 +
		color3 * alpha3
		, 1.0);
}