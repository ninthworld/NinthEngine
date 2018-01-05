#version 330

layout(location=0) in vec3 in_position;
layout(location=1) in vec3 in_normal;
layout(location=2) in vec3 in_tangent;
layout(location=3) in vec2 in_texCoord;

out vec3 vs_normal;
out vec2 vs_texCoord;
out vec3 vs_light;

layout(std140) uniform CameraMatrix {
	mat4 projMatrix;
	mat4 viewMatrix;
	mat4 modelMatrix;
	vec3 cameraPos;
};

layout(std140) uniform Light {
	vec4 lightPosition;
	vec4 lightDiffuse;
	vec4 lightAmbient;
};

uniform sampler2D textureDisp;

float getDispHeight(vec2 pos){
	return texture(textureDisp, pos).r * 0.05;
}

//   | a |  
// --+---+--
// b | e | c
// --+---+--
//   | d | 
vec3 getDispNormal(vec2 pos){
	float x = 1 / 16.0;
	vec3 a = vec3(0, getDispHeight(pos + vec2(0, -x)), -1);
	vec3 b = vec3(-1, getDispHeight(pos + vec2(-x, 0)), 0);
	vec3 c = vec3 
}

void main(){

	vec3 normal = normalize(in_normal);

	// Displacement
	float height = 
	vec3 displace = height * normal; 

	mat4 modelViewMatrix = viewMatrix * modelMatrix;
	gl_Position = projMatrix * modelViewMatrix * vec4(in_position + displace, 1.0);
	vs_normal = normal;
	vs_texCoord = in_texCoord;
	

	vec3 tangent = normalize(in_tangent);
	vec3 bitangent = normalize(cross(normal, tangent));  

	mat3 tangentSpace = mat3(
		tangent.x, bitangent.x, normal.x,
		tangent.y, bitangent.y, normal.y,
		tangent.z, bitangent.z, normal.z);

	vs_light = tangentSpace * lightPosition.xyz;
}