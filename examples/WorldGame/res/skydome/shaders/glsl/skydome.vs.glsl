#version 400

layout(location=0) in vec3 in_position;
layout(location=1) in vec2 in_texCoord;

out vec3 vs_position;
out vec2 vs_texCoord;
out float vs_blend;

layout(std140) uniform Camera {
	mat4 camViewProj;
	vec4 camPosition;
};

void main() {
	
	vs_position = in_position;
	vs_texCoord = in_texCoord;
	vs_blend = clamp(in_position.y / 0.005, 0, 1);
	
	gl_Position = camViewProj * vec4(in_position * 400 + vec3(0, -100, 0) + camPosition.xyz, 1.0);
}