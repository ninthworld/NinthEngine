#version 400

layout(location=0) in vec2 in_position;

out vec2 vs_texCoord;

layout(std140) uniform Camera {
	mat4 viewMatrix;
	mat4 viewProjMatrix;
	vec4 camPosition;
	vec4 frustumPlanes[6];
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

uniform sampler2D heightmap;

void main() {
	
	vec2 localPos = (localMatrix * vec4(in_position.x, 0.0, in_position.y, 1.0)).xz;
	
	vs_texCoord = localPos;
	
	float height = 0.0;
	height += texture(heightmap, vs_texCoord).r;
	
	gl_Position = worldMatrix * vec4(localPos.x, height, localPos.y, 1.0);
}