// Original shader from https://github.com/McNopper/OpenGL/blob/master/Example28/shader/ssao.frag.glsl
// Rewritten to work with existing code

#version 400

#define KERNEL_SIZE		32
#define RADIUS			1
#define MIN_DISTANCE	0.0001
#define MAX_DISTANCE	0.005

in vec2 vs_texCoord;

out vec4 out_fragColor;

layout(std140) uniform Window {
	vec4 windowSize;
};

layout(std140) uniform CameraProj {
	mat4 projMatrix;
	mat4 invProjMatrix;
};

layout(std140) uniform SSAO {
	vec4 kernel[KERNEL_SIZE];
};

uniform sampler2D colorTexture;
uniform sampler2D normalTexture;
uniform sampler2DMS depthTexture;
uniform sampler2D noiseTexture;

vec4 getViewPos(vec2 texCoord) {

	vec2 pos = texCoord * 2.0 - 1.0;
	
	float z = texelFetch(depthTexture, ivec2(texCoord * windowSize.xy), 2).r * 2.0 - 1.0;

	vec4 posProj = vec4(pos, z, 1.0);

	vec4 posView = invProjMatrix * posProj;

	posView /= posView.w;

	return posView;
}

void main() {
	
	vec4 posView = getViewPos(vs_texCoord);

	vec3 normalView = normalize(texture(normalTexture, vs_texCoord).xyz * 2.0 - 1.0);

	vec3 randomVector = normalize(vec3(texture(noiseTexture, vs_texCoord * windowSize.xy / 4.0).xy, 0.0) * 2.0 - 1.0);

	vec3 tangentView = normalize(randomVector - dot(randomVector, normalView) * normalView);

	vec3 bitangentView = cross(normalView, tangentView);

	mat3 kernelMatrix = mat3(tangentView, bitangentView, normalView);

	float occlusion = 0.0;

	for (int i = 0; i < KERNEL_SIZE; ++i) {
		
		vec3 sampleVectorView = kernelMatrix * kernel[i].xyz;
		
		vec4 samplePointView = posView + RADIUS * vec4(sampleVectorView, 0.0);

		vec4 samplePointNDC = projMatrix * samplePointView;

		samplePointNDC /= samplePointNDC.w;

		vec2 samplePointTexCoord = samplePointNDC.xy * 0.5 + 0.5;

		float zSceneNDC = texelFetch(depthTexture, ivec2(samplePointTexCoord * windowSize.xy), 0).r * 2.0 - 1.0;

		float delta = samplePointNDC.z - zSceneNDC;

		if (delta > MIN_DISTANCE && delta < MAX_DISTANCE) {
			occlusion += 1.0;
		}
	}

	occlusion = 1.0 - occlusion / float(KERNEL_SIZE);

	out_fragColor = vec4(occlusion.xxx, 1.0);
}