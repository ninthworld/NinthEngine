#version 330

in vec2 vs_texCoord;

out vec4 out_fragColor;

uniform sampler2D texture0;

void main() {

	vec3 texColor = texture(texture0, vs_texCoord).rgb;

	out_fragColor = vec4(texColor, 1.0);
}