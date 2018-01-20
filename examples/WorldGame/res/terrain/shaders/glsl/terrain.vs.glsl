#version 400

layout(location=0) in vec2 in_position;

out vec2 vs_texCoord;

layout(std140) uniform Terrain {
	vec4 terrainScale;
};

layout(std140) uniform Node {
	vec2 nodeLocalPos;
	float nodeScale;
	int nodeLod;
	vec4 nodeNeighborLods;
};

void main() {

	vec2 localPos = nodeLocalPos + (in_position * nodeScale);
	
	vec3 worldPos = vec3(localPos.x - 0.5, 0, localPos.y - 0.5) * terrainScale.xyz;
	
	vs_texCoord = localPos;
	gl_Position = vec4(worldPos, 1);
}