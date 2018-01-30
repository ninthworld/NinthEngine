#version 400

layout(location=0) in vec3 in_position;
layout(location=1) in vec3 in_normal;
layout(location=2) in float in_material;

out vec4 vs_colorTexture;
out vec3 vs_normal;
out vec3 vs_position;

layout(std140) uniform Camera {
	mat4 camViewProj;
	vec4 camPosition;
};

layout(std140) uniform Chunk {
	vec4 chunkPosition;
};

uniform sampler2D colorsTexture;

const vec4 colorTextures[4] = vec4[](
	vec4(0, 0, 0, 0),				// NULL
	vec4(0, 0, 0, 1),				// Dirt Texture
	vec4(47, 111, 48, 0) / 255.0,	// Grass 1
	vec4(64, 151, 64, 0) / 255.0	// Grass 2
);

void main() {
	
	if (in_material < 32) {
		vs_colorTexture = vec4(0, 0, 0, in_material);
	}
	else {
		vs_colorTexture = vec4(texelFetch(colorsTexture, ivec2(int(in_material) % 32, floor(in_material / 32) - 1), 0).rgb, 0);
	}
	// vs_colorTexture = colorTextures[int(in_material)];
	vs_normal = (in_normal + 1.0) / 2.0;
	vs_position = in_position + chunkPosition.xyz;
	gl_Position = camViewProj * vec4(in_position + chunkPosition.xyz, 1.0);
}