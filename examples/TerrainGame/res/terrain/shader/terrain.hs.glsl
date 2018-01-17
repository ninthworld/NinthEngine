#version 400

layout(vertices=4) out;

in vec2 vs_texCoord[];

out vec2 hs_texCoord[];

layout(std140) uniform Node {
	mat4 localMatrix;
	vec4 neighbors;
	vec2 location;
	vec2 index;
	float size;
	int lod;
	vec2 _nPadding;
};

float tessLevel(int _lod) {
	return mix(16, gl_MaxTessGenLevel, float(_lod) / 8.0);
}

void main() {
	
	if (gl_InvocationID == 0) {
		
		vec4 side = mix(vec4(tessLevel(lod)), vec4(tessLevel(lod + 1) * 2), neighbors); 

		gl_TessLevelOuter[2] = side.x;
		gl_TessLevelOuter[0] = side.y;
		gl_TessLevelOuter[1] = side.z;
		gl_TessLevelOuter[3] = side.w;
		
		gl_TessLevelInner[0] = (gl_TessLevelOuter[3] + gl_TessLevelOuter[1]) / 4.0;
		gl_TessLevelInner[1] = (gl_TessLevelOuter[2] + gl_TessLevelOuter[0]) / 4.0;				
	}

	hs_texCoord[gl_InvocationID] = vs_texCoord[gl_InvocationID];

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}