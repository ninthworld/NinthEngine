#version 400

layout(location=0) in vec3 in_position;
layout(location=1) in vec3 in_color;
layout(location=2) in vec3 in_normal;

out vec3 vs_color;
flat out vec3 vs_normal;

layout(std140) uniform Camera {
	mat4 camViewProj;
	vec4 camPosition;
};

layout(std140) uniform Model {
	mat4 modelTransform;
};

void main() {
	
	vs_color = in_color;
	vs_normal = (normalize(in_normal) + 1.0) / 2.0;
	gl_Position = camViewProj * modelTransform * vec4(in_position, 1.0);
}