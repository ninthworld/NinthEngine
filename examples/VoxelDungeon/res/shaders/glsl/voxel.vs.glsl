#version 400

layout(location=0) in vec3 in_position;
layout(location=1) in vec3 in_normal;
layout(location=2) in float in_material;

out vec4 vs_material;
out vec3 vs_normal;
out vec3 vs_position;

layout(std140) uniform Camera {
	mat4 camViewProj;
	vec4 camPosition;
};

layout(std140) uniform Chunk {
	vec4 chunkPosition;
};

const vec4 Materials[4] = vec4[](
	vec4(0, 0, 0, 0),				// NULL
	//vec4(0, 0, 0, 1),				// Stone Floor	[TEXTURE]
	//vec4(0, 0, 0, 2),				// Stone Wall	[TEXTURE]
	vec4(1, 0, 0, 0),
	vec4(0, 1, 0, 0),
	vec4(100, 70, 40, 0) / 255.0	// Stone Wall Pillar Foundation
);

void main() {
	
	vs_material = Materials[int(in_material)];
	vs_normal = (in_normal + 1.0) / 2.0;
	vs_position = in_position + chunkPosition.xyz;
	gl_Position = camViewProj * vec4(in_position + chunkPosition.xyz, 1.0);
}