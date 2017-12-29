#version 330

layout(location=0) in vec3 in_position;
layout(location=1) in vec2 in_texCoord;

out vec2 vs_texCoord;

layout(std140) uniform ModelViewProjMatrix {
	mat4 modelViewProjMatrix;
};

void main(){
	
	vs_texCoord = in_texCoord;
	
	gl_Position = modelViewProjMatrix * vec4(in_position, 1.0);
}