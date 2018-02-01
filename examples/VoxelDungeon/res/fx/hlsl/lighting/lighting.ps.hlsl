// TODO: Fix this
//#define USE_MSAA

#define MIN_VIEW 0.99984
#define MAX_VIEW 0.99988

#define SAMPLES	4

struct VertexOut {
	float2 texCoord : TEXCOORD;
	float4 svPosition : SV_POSITION;
};

struct PixelOut {
	float4 color : SV_TARGET0;
};

cbuffer Camera : register(b0) {
	float4x4 camViewProj;
	float4 camPosition;
};

cbuffer Window : register(b1) {
	float4 windowSize;
};

SamplerState texSampler : register(s0);

#ifdef USE_MSAA
Texture2DMS<float4, SAMPLES> colorTexture : register(t0);
Texture2D<float4, SAMPLES> normalTexture : register(t1);
Texture2D<float, SAMPLES> depthTexture : register(t2);
#else
Texture2D colorTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D depthTexture : register(t2);
#endif
Texture2D ssaoTexture : register(t3);

PixelOut main(VertexOut IN) {
	PixelOut OUT;

	float3 lightDir = normalize(float3(0.8, 1.2, 0.6));

	float cosTheta = 0;
	float3 color = float3(0, 0, 0);
#ifdef USE_MSAA
	for (int i = 0; i < SAMPLES; ++i) {
		float depth = depthTexture.SampleLevel(texSampler, IN.texCoord, i).r;
		float3 normal = normalTexture.SampleLevel(texSampler, IN.texCoord, i).rgb * 2.0 - 1.0;

		float3 diffuse = colorTexture.SampleLevel(texSampler, IN.texCoord, i).rgb;
		diffuse *= max(dot(normal, lightDir), 0.3);

		color += diffuse;
	}
	color /= SAMPLES;
#else
	float3 normal = normalTexture.Sample(texSampler, IN.texCoord).rgb * 2.0 - 1.0;
	float3 diffuse = colorTexture.Sample(texSampler, IN.texCoord).rgb;
	diffuse *= max(dot(normal, lightDir), 0.3);
	color += diffuse;
#endif

	float3 ssao = ssaoTexture.Sample(texSampler, IN.texCoord).rgb;
	color *= ssao;

	OUT.color = float4(color, 1);

	return OUT;
}