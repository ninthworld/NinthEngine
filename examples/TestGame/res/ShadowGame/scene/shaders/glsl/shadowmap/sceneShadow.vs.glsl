#version 400

layout(location=0) in vec3 in_position;
layout(location=1) in vec3 in_color;
layout(location=2) in vec3 in_normal;

layout(std140) uniform ShadowCamera {
	mat4 camViewProj;
	vec4 camPosition;
};

layout(std140) uniform Model {
	mat4 modelTransform;
};

void main() {
	
	gl_Position = camViewProj * modelTransform * vec4(in_position, 1.0);
}