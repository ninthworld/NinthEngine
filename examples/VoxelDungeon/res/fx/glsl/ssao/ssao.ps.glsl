#version 400

// Original shader from https://github.com/McNopper/OpenGL/blob/master/Example28/shader/ssao.frag.glsl
// Rewritten to work with existing code

// TODO: Fix This
//#define USE_MSAA

#define KERNEL_SIZE		32
#define RADIUS			1
#define MIN_DISTANCE	0.00001
#define MAX_DISTANCE	0.005

in vec2 vs_texCoord;

layout(location=0) out vec4 ps_color;

layout(std140) uniform CameraExt {
	mat4 camView;
	mat4 camProj;
	mat4 camInvView;
	mat4 camInvProj;
};

layout(std140) uniform Window {
	vec4 windowSize;
};

layout(std140) uniform SSAO {
	vec4 ssaoKernel[KERNEL_SIZE];
};

#ifdef USE_MSAA
uniform sampler2DMS normalTexture;
uniform sampler2DMS depthTexture;
#else
uniform sampler2D normalTexture;
uniform sampler2D depthTexture;
#endif
uniform sampler2D noiseTexture;

vec4 getViewPos(vec2 texCoord) {

	vec2 pos = texCoord * 2.0 - 1.0;

#ifdef USE_MSAA
	float z = texelFetch(depthTexture, ivec2(texCoord * windowSize.xy), 0).r * 2.0 - 1.0;
#else
	float z = texture(depthTexture, texCoord).r * 2.0 - 1.0;
#endif

	vec4 posProj = vec4(pos, z, 1.0);

	vec4 posView = camInvProj * posProj;

	posView /= posView.w;

	return posView;
}

void main() {

	vec4 posView = getViewPos(vs_texCoord);

#ifdef USE_MSAA
	vec3 normalView = normalize(texelFetch(normalTexture, ivec2(vs_texCoord * windowSize.xy), 0).xyz * 2.0 - 1.0);
#else
	vec3 normalView = normalize(texture(normalTexture, vs_texCoord).xyz * 2.0 - 1.0);
#endif

	vec3 randomVector = normalize(vec3(texture(noiseTexture, vs_texCoord * windowSize.xy / 4.0).xy, 0.0) * 2.0 - 1.0);

	vec3 tangentView = normalize(randomVector - dot(randomVector, normalView) * normalView);

	vec3 bitangentView = cross(normalView, tangentView);

	mat3 kernelMatrix = mat3(tangentView, bitangentView, normalView);

	float occlusion = 0.0;

	for (int i = 0; i < KERNEL_SIZE; ++i) {
		
		vec3 sampleVectorView = kernelMatrix * ssaoKernel[i].xyz;
		
		vec4 samplePointView = posView + RADIUS * vec4(sampleVectorView, 0.0);

		vec4 samplePointNDC = camProj * samplePointView;

		samplePointNDC /= samplePointNDC.w;

		vec2 samplePointTexCoord = samplePointNDC.xy * 0.5 + 0.5;

#ifdef USE_MSAA
		float zSceneNDC = texelFetch(depthTexture, ivec2(vs_texCoord * windowSize.xy), 0).r * 2.0 - 1.0;
#else
		float zSceneNDC = texture(depthTexture, vs_texCoord).r * 2.0 - 1.0;
#endif

		float delta = samplePointNDC.z - zSceneNDC;

		if (delta > MIN_DISTANCE && delta < MAX_DISTANCE) {
			occlusion += 1.0;
		}
	}

	occlusion = 1.0 - occlusion / float(KERNEL_SIZE);

	ps_color = vec4(vec3(occlusion), 1.0);

	//ps_color = vec4(position, 1.0);
}