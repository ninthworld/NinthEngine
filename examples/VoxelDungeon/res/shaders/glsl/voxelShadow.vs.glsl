#version 400

layout(location=0) in vec3 in_position;
layout(location=1) in vec3 in_normal;
layout(location=2) in float in_material;

layout(std140) uniform Camera {
	mat4 camViewProj;
	vec4 camPosition;
};

layout(std140) uniform Chunk {
	vec4 chunkPosition;
};

void main() {

	gl_Position = camViewProj * vec4(in_position + chunkPosition.xyz, 1.0);
}