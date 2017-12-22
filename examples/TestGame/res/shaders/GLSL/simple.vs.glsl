#version 330

layout (location=0) in vec3 in_position;

layout (std140) uniform ViewProjMatrix {
	mat4 viewProjMatrix;
};

layout (std140) uniform ModelMatrix {
	mat4 modelMatrix;
};

void main(){

	//gl_Position = vec4(in_position, 1.0);
	gl_Position = viewProjMatrix * modelMatrix * vec4(in_position, 1.0);
}