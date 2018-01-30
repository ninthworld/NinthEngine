#version 400

in vec4 vs_colorTexture;
in vec3 vs_normal;
in vec3 vs_position;

layout(location=0) out vec4 ps_color;
layout(location=1) out vec4 ps_normal;

uniform sampler2D dirtTexture;

void main() {
	
	vec3 color = vs_colorTexture.rgb;

	if (vs_colorTexture.a > 0.0) {
		color = texture(dirtTexture, vs_position.xz / 16.0).rgb;
	}

	ps_color = vec4(color, 1.0);
	ps_normal = vec4(vs_normal, 1.0);

	//vec3 normal = (vs_normal + 1.0) / 2.0;
	//ps_color = vec4(normal, 1.0);
}