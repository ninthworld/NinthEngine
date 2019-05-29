#version 400

in vec3 vs_color;

layout(location=0) out vec4 out_color;

void main() {
	out_color = vec4(vs_color, 1.0);
}