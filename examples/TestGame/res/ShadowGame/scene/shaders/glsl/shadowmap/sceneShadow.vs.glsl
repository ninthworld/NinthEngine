#version 400

layout(location=0) in vec3 in_position;
layout(location=1) in vec3 in_color;
layout(location=2) in vec3 in_normal;

layout(std140) uniform ShadowMap {
	mat4 shadowCamViewProj;
	vec4 shadowCamPosition;
};

layout(std140) uniform Model {
	mat4 modelTransform;
};

void main() {
	
	gl_Position = shadowCamViewProj * modelTransform * vec4(in_position, 1.0);
}