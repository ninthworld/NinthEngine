#version 400

layout(location=0) in vec2 in_position;

layout(std140) uniform Terrain {
	vec4 terrainScale;
};

layout(std140) uniform Node {
	vec2 nodeLocalPos;
	float nodeScale;
	int nodeLod;
};

layout(std140) uniform ShadowMap {
	mat4 shadowViewProj;
};

uniform sampler2D heightmap;

void main() {

	vec2 localPos = nodeLocalPos + (in_position * nodeScale);

	float height = texture(heightmap, localPos).r;

	vec3 worldPos = vec3(localPos.x - 0.5, height, localPos.y - 0.5) * terrainScale.xyz;
	
	gl_Position = shadowViewProj * vec4(worldPos, 1);
}