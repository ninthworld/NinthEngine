#version 400

in vec2 vs_texCoord;

layout(location=0) out vec4 ps_color;

uniform sampler2D normalmap;

void main() {

	vec3 normal = normalize(texture(normalmap, vs_texCoord).rgb * 2.0 - 1.0);

	vec3 lightDir = normalize(vec3(1));
	float cosTheta = max(dot(normal, lightDir), 0.1);

	ps_color = vec4(vec3(1, 0, 0) * cosTheta, 1);
}