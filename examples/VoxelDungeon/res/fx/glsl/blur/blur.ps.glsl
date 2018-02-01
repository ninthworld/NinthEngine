#version 400

in vec2 vs_texCoord;

layout(location=0) out vec4 ps_color;

layout(std140) uniform Window {
	vec4 windowSize;
};

uniform sampler2D colorTexture;

void main() {

	vec3 color;
	for (int i = -2; i <= 2; ++i) {
		for (int j = -2; j <= 2; ++j) {
			vec2 offset = vs_texCoord + vec2(i, j) * vec2(1.0 / windowSize.xy);
			color += texture(colorTexture, offset).rgb;
		}
	}
	color /= 25.0;

	ps_color = vec4(color, 1.0);
}