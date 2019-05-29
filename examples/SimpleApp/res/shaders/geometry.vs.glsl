#version 420

layout(location=0) in vec3 in_position;
layout(location=1) in vec3 in_color;

out vec3 vs_color;

layout(std140, binding=0) uniform Camera {
	mat4 viewMatrix;
	mat4 projMatrix;
	mat4 viewProjMatrix;
} u_camera;

void main() {
	vs_color = in_color;
	gl_Position = u_camera.viewProjMatrix * vec4(in_position, 1.0);
}