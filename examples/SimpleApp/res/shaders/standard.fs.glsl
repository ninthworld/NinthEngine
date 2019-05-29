#version 420

in vec3 vs_normal;
in vec3 vs_texCoord;

layout(location=0) out vec4 out_color;

void main() {
	out_color = vec4(1.0, 0.0, 0.0, 1.0);
}