#version 330

layout(location = 0) in vec2 in_position;
layout(location = 1) in vec2 in_texCoord;

out vec2 vs_texCoord;

void main(){

	vs_texCoord = in_texCoord;

	gl_Position = vec4(in_position, 0.0, 1.0);
}