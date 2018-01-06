#version 400

layout(location=0) in vec3 in_position;
layout(location=1) in vec2 in_texCoord;

out vec2 vs_texCoord;
out float vs_blend;

layout(std140) uniform Camera {
	mat4 viewMatrix;
	mat4 viewProjMatrix;
	vec4 camPosition;
};

layout(std140) uniform Skydome {
	vec4 skyColor;
	vec4 scale;
};

void main(){
	
	vs_texCoord = in_texCoord;
	vs_blend = pow(1.0 - in_position.y, 2.0);
	
	gl_Position = viewProjMatrix * vec4(in_position * scale.xyz + camPosition.xyz, 1.0);
}