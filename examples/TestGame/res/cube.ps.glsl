#version 400

in vec3 vs_color;

out vec4 out_fragColor;

void main() {

	out_fragColor = vec4(vs_color, 1.0);
}