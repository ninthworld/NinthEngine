#version 400

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

in vec2 vs_texCoord[];

out vec2 gs_texCoord;
out vec3 gs_position;
out vec3 gs_tangent;

layout(std140) uniform Camera {
	mat4 camViewProj;
	vec4 camPosition;
};

layout(std140) uniform Terrain {
	vec4 terrainScale;
};

vec3 calcTangent() {

	vec3 v0 = gl_in[0].gl_Position.xyz;
	vec3 v1 = gl_in[1].gl_Position.xyz;
	vec3 v2 = gl_in[2].gl_Position.xyz;

	vec3 e1 = v1 - v0;
	vec3 e2 = v2 - v0;
	
	vec2 uv0 = vs_texCoord[0];
	vec2 uv1 = vs_texCoord[1];
	vec2 uv2 = vs_texCoord[2];

	vec2 deltaUV1 = uv1 - uv0;
	vec2 deltaUV2 = uv2 - uv0;
	
	float r = 1.0 / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
	
	return normalize((e1 * deltaUV2.y - e2 * deltaUV1.y) * r);
}

void main() {
	
	for (int i = 0; i < gl_in.length(); ++i) {
		gs_texCoord = vs_texCoord[i] * terrainScale.x;
		gs_position = gl_in[i].gl_Position.xyz;
		gs_tangent = calcTangent();
		gl_Position = camViewProj * gl_in[i].gl_Position;
		EmitVertex();
	}

	EndPrimitive();
}