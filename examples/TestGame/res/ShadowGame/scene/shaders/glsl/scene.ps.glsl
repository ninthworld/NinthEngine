#version 400

in vec3 vs_color;
in vec3 vs_normal;

layout(location=0) out vec4 ps_color;
layout(location=1) out vec4 ps_normal;

void main() {
	
	ps_color = vec4(vs_color, 1.0);
	ps_normal = vec4(vs_normal, 1.0);
}