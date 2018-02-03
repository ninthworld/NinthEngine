#version 400

in vec4 vs_material;
in vec3 vs_normal;
in vec3 vs_position;

layout(location=0) out vec4 ps_color;
layout(location=1) out vec4 ps_normal;

uniform sampler2D stoneFloor;
uniform sampler2D dirtFloor;
uniform sampler2D stoneWall;
uniform sampler2D stonePillarSmall;

void main() {
	
	vec3 color;
	switch(int(vs_material.a)) {
	// Stone Floor
	case 1: color = texture(stoneFloor, vs_position.xz / 16.0).rgb; break;
	
	// Dirt Floor
	case 2: color = texture(dirtFloor, vs_position.xz / 16.0).rgb; break;

	// Stone Wall (Facing Z)
	case 8: color = texture(stoneWall, vec2(vs_position.x, -vs_position.y + 1.0) / 16.0).rgb; break;

	// Stone Wall (Facing X)
	case 9: color = texture(stoneWall, vec2(vs_position.z, -vs_position.y + 1.0) / 16.0).rgb; break;
	
	// Stone Pillar Small
	case 10: color = texture(stonePillarSmall, vec2(vs_position.x + vs_position.z, -vs_position.y + 1.0) / vec2(4.0, 16.0)).rgb; break;

	// No Texture
	default: color = vs_material.rgb; break;
	}

	ps_color = vec4(color, 1.0);
	ps_normal = vec4(vs_normal, 1.0);
}