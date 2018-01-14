// Original shader from https://github.com/McNopper/OpenGL/blob/master/Example28/shader/ssao.frag.glsl
// Rewritten in HLSL and made to work with existing code

#define KERNEL_SIZE		32
#define RADIUS			1
#define MIN_DISTANCE	0.0001
#define MAX_DISTANCE	0.005

struct VertexOut {
	float2 texCoord : TEXCOORD;
	float4 svPosition : SV_POSITION;
};

cbuffer Window : register(b0) {
	float4 windowSize;
};

cbuffer CameraProj : register(b1) {
	float4x4 projMatrix;
	float4x4 invProjMatrix;
};

cbuffer SSAO : register(b2) {
	float4 kernel[32];
};

SamplerState texSampler : register(s0);

Texture2D normalTexture : register(t0);
Texture2DMS<float, 4> depthTexture : register(t1);
Texture2D noiseTexture : register(t2);

float4 getViewPos(float2 texCoord) {

	float2 pos = texCoord * 2.0 - 1.0;
		
	float z = depthTexture.Load(texCoord * windowSize.xy, 0).r * 2.0 - 1.0;

	float4 posProj = float4(pos, z, 1.0);

	float4 posView = mul(invProjMatrix, posProj);

	posView /= posView.w;

	return posView;
}

float4 main(VertexOut IN) : SV_TARGET {
	
	float4 posView = getViewPos(IN.texCoord);
	
	float3 normalView = normalize(normalTexture.Sample(texSampler, IN.texCoord).xyz * 2.0 - 1.0);

	float3 randomVector = normalize(float3(noiseTexture.Sample(texSampler, IN.texCoord * windowSize.xy / 4.0).xy, 0.0) * 2.0 - 1.0);

	float3 tangentView = normalize(randomVector - dot(randomVector, normalView) * normalView);

	float3 bitangentView = cross(normalView, tangentView);

	float3x3 kernelMatrix = float3x3(tangentView, bitangentView, normalView);

	float occlusion = 0.0;

	for (int i = 0; i < KERNEL_SIZE; ++i) {
		
		float3 sampleVectorView = mul(kernelMatrix, kernel[i].xyz);
		
		float4 samplePointView = posView + RADIUS * float4(sampleVectorView, 0.0);

		float4 samplePointNDC = mul(projMatrix, samplePointView);

		samplePointNDC /= samplePointNDC.w;

		float2 samplePointTexCoord = samplePointNDC.xy * 0.5 + 0.5;

		float zSceneNDC = depthTexture.Load(samplePointTexCoord * windowSize.xy, 0).r * 2.0 - 1.0;

		float delta = samplePointNDC.z - zSceneNDC;

		if (delta > MIN_DISTANCE && delta < MAX_DISTANCE) {
			occlusion += 1.0;
		}
	}

	occlusion = 1.0 - occlusion / float(KERNEL_SIZE);

	return float4(occlusion.xxx, 1.0);
}