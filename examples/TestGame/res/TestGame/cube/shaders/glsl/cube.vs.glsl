#version 400

layout(location=0) in vec3 in_position;
layout(location=1) in vec3 in_color;

out vec3 vs_color;

layout(std140) uniform Camera {
	mat4 camViewProj;
	vec4 camPosition;
};

void main() {

	vs_color = in_color;

	gl_Position = camViewProj * vec4(in_position, 1.0);
}