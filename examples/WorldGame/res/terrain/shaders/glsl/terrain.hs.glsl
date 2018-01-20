#version 400

layout(vertices=16) out;

in vec2 vs_texCoord[];

out vec2 hs_texCoord[];

layout(std140) uniform Camera {
	mat4 viewMatrix;
	mat4 viewProjMatrix;
	vec4 camPosition;
};

layout(std140) uniform Node {
	vec2 nodeLocalPos;
	float nodeScale;
	int nodeLod;
	vec4 nodeNeighborLods;
};

float getTessLevel(float lod) {
	return mix(16, gl_MaxTessGenLevel, float(lod) / 8.0);
}

void main() {

	#define id gl_InvocationID

	if (id == 0) {
		
		float side[4];
		for (int i = 0; i < 4; ++i) {
			if (nodeNeighborLods[i] > nodeLod) {
				side[i] = getTessLevel(nodeNeighborLods[i]) * 2.0;
			}
			else {
				side[i] = getTessLevel(nodeLod);
			}
		}

		gl_TessLevelOuter[0] = side[3];
		gl_TessLevelOuter[1] = side[0];
		gl_TessLevelOuter[2] = side[2];
		gl_TessLevelOuter[3] = side[1];
		
		gl_TessLevelInner[0] = (gl_TessLevelOuter[3] + gl_TessLevelOuter[1]) / 2.0;
		gl_TessLevelInner[1] = (gl_TessLevelOuter[2] + gl_TessLevelOuter[0]) / 2.0;
	}

	hs_texCoord[id] = vs_texCoord[id];
	gl_out[id].gl_Position = gl_in[id].gl_Position;
}