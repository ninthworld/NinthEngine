#version 420

layout(location=0) in vec3 in_position;
layout(location=1) in vec3 in_normal;
layout(location=2) in vec2 in_texCoord;

out vec3 vs_normal;
out vec2 vs_texCoord;

layout(std140, binding=0) uniform Camera {
	mat4 viewMatrix;
	mat4 projMatrix;
	mat4 viewProjMatrix;
} u_camera;

void main() {
	gl_Position = u_camera.viewProjMatrix * vec4(in_position, 1.0);
	
	vs_normal = in_normal;
	vs_texCoord = in_texCoord;
}