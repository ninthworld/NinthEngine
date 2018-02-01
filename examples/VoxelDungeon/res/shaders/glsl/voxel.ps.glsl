#version 400

in vec4 vs_material;
in vec3 vs_normal;
in vec3 vs_position;

layout(location=0) out vec4 ps_color;
layout(location=1) out vec4 ps_normal;

uniform sampler2D stoneFloor;
uniform sampler2D stoneWall;

void main() {
	
	vec3 color = vs_material.rgb;

	if (vs_material.a == 1) {
		color = texture(stoneFloor, vs_position.xz / 32.0).rgb;
	}
	else if (vs_material.a == 2) {
		vec2 texCoord;
		if (vs_normal.x > 0) texCoord = vs_position.xy / vec2(64, -64);
		if (vs_normal.x < 0) texCoord = vs_position.xy / vec2(64, -64);
		if (vs_normal.z > 0) texCoord = vs_position.zy / vec2(64, -64);
		if (vs_normal.z < 0) texCoord = vs_position.zy / vec2(64, -64);
		color = texture(stoneWall, texCoord).rgb;
	}

	ps_color = vec4(color, 1.0);
	ps_normal = vec4(vs_normal, 1.0);
}