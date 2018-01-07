#version 400

layout(quads, equal_spacing, cw) in;

in vec2 hs_texCoord[];

out vec2 ds_texCoord;

layout(std140) uniform Terrain {
	mat4 worldMatrix;
	float scaleXZ;
	float scaleY;
	float detailRangeNear;
	float detailRangeFar;
	float lodRange[8];
};

uniform sampler2D heightmap;

void main() {

	float u0 = gl_TessCoord.x;
	float v0 = gl_TessCoord.y;
	float u1 = (1 - u0);
	float v1 = (1 - v0);

	vec4 position = 
		u1 * v1 * gl_in[12].gl_Position +
		u0 * v1 * gl_in[0].gl_Position +
		u0 * v0 * gl_in[3].gl_Position +
		u1 * v0 * gl_in[15].gl_Position;
	
	vec2 texCoord =
		u1 * v1 * hs_texCoord[12] +
		u0 * v1 * hs_texCoord[0] +
		u0 * v0 * hs_texCoord[3] +
		u1 * v0 * hs_texCoord[15];

	float height = 0;
	height += texture(heightmap, texCoord).r;
	height *= scaleY;

	position.y = height;

	ds_texCoord = texCoord * 1000; // TODO: Replace with texDetail

	gl_Position = position;
}