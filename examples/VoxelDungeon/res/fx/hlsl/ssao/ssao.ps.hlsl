// Original shader from https://github.com/McNopper/OpenGL/blob/master/Example28/shader/ssao.frag.glsl
// Rewritten to work with existing code
// Converted to HLSL from GLSL

// TODO: Fix This
//#define USE_MSAA

#define KERNEL_SIZE		32
#define RADIUS			1
#define MIN_DISTANCE	0.00001
#define MAX_DISTANCE	0.005

#define SAMPLES 4

struct VertexOut {
	float2 texCoord : TEXCOORD;
	float4 svPosition : SV_POSITION;
};

struct PixelOut {
	float4 color : SV_TARGET0;
};

cbuffer CameraExt : register(b0) {
	float4x4 camView;
	float4x4 camProj;
	float4x4 camInvView;
	float4x4 camInvProj;
};

cbuffer Window : register(b1) {
	float4 windowSize;
};

cbuffer Window : register(b2) {
	float4 ssaoKernel[KERNEL_SIZE];
};

SamplerState texSampler : register(s0);

#ifdef USE_MSAA
Texture2DMS<float4, SAMPLES> normalTexture : register(t0);
Texture2DMS<float, SAMPLES> depthTexture : register(t1);
#else
Texture2D normalTexture : register(t0);
Texture2D depthTexture : register(t1);
#endif
Texture2D noiseTexture : register(t2);

float4 getViewPos(float2 texCoord) {

	float2 pos = texCoord * 2.0 - 1.0;

#ifdef USE_MSAA
	float z = depthTexture.SampleLevel(texSampler, texCoord, 0).r * 2.0 - 1.0;
#else
	float z = depthTexture.Sample(texSampler, texCoord).r * 2.0 - 1.0;
#endif

	float4 posProj = float4(pos, z, 1.0);

	float4 posView = mul(camInvProj, posProj);

	posView /= posView.w;

	return posView;
}

PixelOut main(VertexOut IN) {
	PixelOut OUT;

	float4 posView = getViewPos(IN.texCoord);

#ifdef USE_MSAA
	float3 normalView = normalize(normalTexture.SampleLevel(texSampler, IN.texCoord, 0).xyz * 2.0 - 1.0);
#else
	float3 normalView = normalize(normalTexture.Sample(texSampler, IN.texCoord).xyz * 2.0 - 1.0);
#endif

	float3 randomVector = normalize(float3(noiseTexture.Sample(texSampler, IN.texCoord * windowSize.xy / 4.0).xy, 0.0) * 2.0 - 1.0);

	float3 tangentView = normalize(randomVector - dot(randomVector, normalView) * normalView);

	float3 bitangentView = cross(normalView, tangentView);

	float3x3 kernelMatrix = float3x3(tangentView, bitangentView, normalView);

	float occlusion = 0.0;

	for (int i = 0; i < KERNEL_SIZE; ++i) {

		float3 sampleVectorView = mul(kernelMatrix, ssaoKernel[i].xyz);

		float4 samplePointView = posView + RADIUS * float4(sampleVectorView, 0.0);

		float4 samplePointNDC = mul(camProj, samplePointView);

		samplePointNDC /= samplePointNDC.w;

		float2 samplePointTexCoord = samplePointNDC.xy * 0.5 + 0.5;

#ifdef USE_MSAA
		float zSceneNDC = depthTexture.SampleLevel(texSampler, IN.texCoord, 0).r * 2.0 - 1.0;
#else
		float zSceneNDC = depthTexture.Sample(texSampler, IN.texCoord).r * 2.0 - 1.0;
#endif

		float delta = samplePointNDC.z - zSceneNDC;

		if (delta > MIN_DISTANCE && delta < MAX_DISTANCE) {
			occlusion += 1.0;
		}
	}

	occlusion = 1.0 - occlusion / float(KERNEL_SIZE);

	OUT.color = float4(occlusion, occlusion, occlusion, 1.0);
	
	return OUT;
}