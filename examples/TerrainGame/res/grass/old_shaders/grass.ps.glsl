#version 400

in vec3 vs_color;
in vec3 vs_normal;
in vec3 vs_position;

layout(location=0) out vec4 ps_color;
layout(location=1) out vec4 ps_normal;
layout(location=2) out vec4 ps_position;

void main() {	

	//vec3 color = mix(vec3(0.10, 0.14, 0.08), vec3(0.20, 0.40, 0.16), gs_color.x);
	vec3 color = mix(vec3(0.10, 0.18, 0.08), vec3(0.20, 0.48, 0.16), vs_color.x);

	ps_color = vec4(color, 1.0);
	ps_normal = vec4(vs_normal * 0.5 + 1.0, 1.0);
	ps_position = vec4(vs_position, 1.0);
}