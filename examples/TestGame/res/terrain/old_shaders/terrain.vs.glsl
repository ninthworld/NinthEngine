#version 330

layout(location=0) in vec2 in_position;

out vec2 vs_position;
out vec3 vs_normal;

layout(std140) uniform ViewProjMatrix {
	mat4 viewProjMatrix;
};

layout(std140) uniform ModelMatrix {
	mat4 modelMatrix;
};

uniform sampler2D heightTexture;
uniform sampler2D normalTexture;

void main(){

	vec4 pos = modelMatrix * vec4(in_position.x, 0.0, in_position.y, 1.0);
	vec2 adjPos = (pos.xz + 512.0) / 1024.0;
	pos.y = texture(heightTexture, adjPos).r * 64.0f;

	gl_Position = viewProjMatrix * pos;
	vs_position = in_position;
	vs_normal = normalize(texture(normalTexture, adjPos).rgb * 2.0 - 1.0);
}