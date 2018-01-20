#version 400

layout(quads, equal_spacing, ccw) in;

in vec2 hs_texCoord[];

out vec2 ds_texCoord;

layout(std140) uniform Terrain {
	vec4 terrainScale;
};

uniform sampler2D heightmap;
uniform sampler2D fractal01_heightmap;
uniform sampler2D fractal02_heightmap;

void main() {

	float u = gl_TessCoord.x;
	float v = gl_TessCoord.y;
	
	vec2 t0 = mix(hs_texCoord[12], hs_texCoord[0], u);
	vec2 t1 = mix(hs_texCoord[15], hs_texCoord[3], u);
	vec2 texCoord = mix(t0, t1, v);
	
	vec4 p0 = mix(gl_in[12].gl_Position, gl_in[0].gl_Position, u);
	vec4 p1 = mix(gl_in[15].gl_Position, gl_in[3].gl_Position, u);
	vec4 position = mix(p0, p1, v);

	float height = texture(heightmap, texCoord).r;
	height += texture(fractal01_heightmap, texCoord).r / 32.0;
	height += texture(fractal02_heightmap, texCoord).r / 64.0;
	height *= terrainScale.y;

	position.y = height;

	ds_texCoord = texCoord;
	gl_Position = position;
}