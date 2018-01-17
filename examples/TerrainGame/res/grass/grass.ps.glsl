#version 400

in vec3 vs_normal;
in vec2 vs_texCoord;
in float vs_alpha;

out vec4 ps_color;
out vec4 ps_normal;

uniform sampler2D grassColor;
uniform sampler2D grassAlpha;

void main() {

	vec3 color = texture(grassColor, vs_texCoord).rgb;
	float alpha = texture(grassAlpha, vs_texCoord).r;

	ps_normal = vec4(vs_normal, 1);
	ps_color = vec4(color, alpha * vs_alpha);
}