#version 330

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texCoord;

out vec2 vs_texCoord;

uniform mat4 viewProjMatrix;
uniform mat4 modelMatrix;

void main() {
	gl_Position = viewProjMatrix * modelMatrix * vec4(in_position, 1.0);
	vs_texCoord = in_texCoord;
}