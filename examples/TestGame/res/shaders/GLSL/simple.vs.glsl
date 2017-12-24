#version 330

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_color;
layout(location = 2) in vec2 in_texCoord;

out vec3 vs_color;
out vec2 vs_texCoord;

layout (std140) uniform ViewProjMatrix {
	mat4 viewProjMatrix;
};

layout (std140) uniform ModelMatrix {
	mat4 modelMatrix;
};

void main(){

	vs_color = in_color;
	vs_texCoord = in_texCoord;

	gl_Position = viewProjMatrix * modelMatrix * vec4(in_position, 1.0);
	//gl_Position = vec4(in_position, 1.0);
}