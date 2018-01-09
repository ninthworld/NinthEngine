#version 400

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

in vec2 ds_texCoord[];

out vec2 gs_texCoord;
out vec4 gs_viewSpacePos;
out vec3 gs_position;
out vec3 gs_tangent;

layout(std140) uniform Camera {
	mat4 viewMatrix;
	mat4 viewProjMatrix;
	vec4 camPosition;
	vec4 frustumPlanes[6];
};

layout(std140) uniform Terrain {
	mat4 worldMatrix;
	float scaleXZ;
	float scaleY;
	float detailRangeNear;
	float detailRangeFar;
	float lodRange[8];
};

uniform sampler2D material0Disp;
uniform sampler2D material0Alpha;

uniform sampler2D material1Disp;
uniform sampler2D material1Alpha;

uniform sampler2D material2Disp;
uniform sampler2D material2Alpha;

uniform sampler2D material3Disp;
uniform sampler2D material3Alpha;

vec3 tangent;
vec3 displacement[3];

void main() {


	displacement[0] = displacement[1] = displacement[2] = vec3(0);

	float dist = 
		distance(gl_in[0].gl_Position.xyz, camPosition.xyz) + 
		distance(gl_in[1].gl_Position.xyz, camPosition.xyz) + 
		distance(gl_in[2].gl_Position.xyz, camPosition.xyz);
	dist /= 3.0;

	if (dist < detailRangeFar) {
		
		float material0Scale = 4.0;
		float material1Scale = 4.0;
		float material2Scale = 10.0;
		float material3Scale = 10.0;

		float material0Height = 0.0;
		float material1Height = 1.0;
		float material2Height = 3.0;
		float material3Height = 3.0;
		
		vec3 v0 = gl_in[0].gl_Position.xyz;
		vec3 v1 = gl_in[1].gl_Position.xyz;
		vec3 v2 = gl_in[2].gl_Position.xyz;

		vec3 e1 = v1 - v0;
		vec3 e2 = v2 - v0;
	
		vec2 uv0 = ds_texCoord[0];
		vec2 uv1 = ds_texCoord[1];
		vec2 uv2 = ds_texCoord[2];

		vec2 deltaUV1 = uv1 - uv0;
		vec2 deltaUV2 = uv2 - uv0;
	
		float r = 1.0 / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
	
		tangent = normalize((e1 * deltaUV2.y - e2 * deltaUV1.y)*r);

		for (int i = 0; i < gl_in.length(); ++i) {
			
			vec2 mapCoords = (gl_in[i].gl_Position.xz + scaleXZ / 2) / scaleXZ;

			displacement[i] = vec3(0, 1, 0);

			float height = gl_in[i].gl_Position.y;

			float scale = 0;
			scale += texture(material0Disp, ds_texCoord[i] / material0Scale).r * material0Height * texture(material0Alpha, mapCoords).r;
			scale += texture(material1Disp, ds_texCoord[i] / material1Scale).r * material1Height * texture(material1Alpha, mapCoords).r;
			scale += texture(material2Disp, ds_texCoord[i] / material2Scale).r * material2Height * texture(material2Alpha, mapCoords).r;
			scale += texture(material3Disp, ds_texCoord[i] / material3Scale).r * material3Height * texture(material3Alpha, mapCoords).r;

			float attenuation = clamp(-distance(gl_in[i].gl_Position.xyz, camPosition.xyz) / detailRangeNear + 1, 0, 1);
			scale *= attenuation;

			displacement[i] *= scale;
		}
	}

	for (int i = 0; i < gl_in.length(); ++i) {
		
		vec4 worldPos = gl_in[i].gl_Position + vec4(displacement[i], 0);
		gl_Position = viewProjMatrix * worldPos;
		/*
		gl_ClipDistance[0] = dot(gl_Position, frustumPlanes[0]);
		gl_ClipDistance[1] = dot(gl_Position, frustumPlanes[1]);
		gl_ClipDistance[2] = dot(gl_Position, frustumPlanes[2]);
		gl_ClipDistance[3] = dot(gl_Position, frustumPlanes[3]);
		gl_ClipDistance[4] = dot(gl_Position, frustumPlanes[4]);
		gl_ClipDistance[5] = dot(gl_Position, frustumPlanes[5]);
		*/
		
		gs_texCoord = ds_texCoord[i];

		gs_viewSpacePos = viewMatrix * worldPos;

		gs_position = worldPos.xyz;

		gs_tangent = tangent;
		
		EmitVertex();
	}
	
	EndPrimitive();
}