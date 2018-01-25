
#define MIN_VIEW 0.99984
#define MAX_VIEW 0.99988

#define SAMPLES	4

struct VertexOut {
	float2 texCoord : TEXCOORD;
	float4 svPosition : SV_POSITION;
};

struct PixelOut {
	float4 color : SV_TARGET;
};

cbuffer Camera : register(b0) {
	float4x4 camViewProj;
	float4 camPosition;
};

cbuffer Window : register(b1) {
	float4 windowSize;
};

cbuffer Sky : register(b2) {
	float4 sunPosition;
	float4 skyColor;
};

cbuffer ShadowMap : register(b3) {
	float4x4 shadowViewProj;
};

SamplerState texSampler : register(s0);

Texture2DMS<float4> colorTexture : register(t0);
Texture2DMS<float4> normalTexture : register(t1);
Texture2DMS<float4> positionTexture : register(t2);
Texture2DMS<float> depthTexture : register(t3);
Texture2D skydomeTexture : register(t4);
Texture2D shadowTexture : register(t5);

float4x4 biasMatrix = float4x4(
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 0.5, 0.0,
	0.5, 0.5, 0.5, 1.0);

PixelOut main(VertexOut IN) {
	PixelOut OUT;

	float3 lightDir = normalize(sunPosition.xzy);

	float shadow = shadowTexture.Sample(texSampler, IN.texCoord).r;
	float3 skydome = skydomeTexture.Sample(texSampler, IN.texCoord).rgb;

	// Multisampling
	float cosTheta = 0;
	float3 color = float3(0, 0, 0);
	for (int i = 0; i < SAMPLES; ++i) {
		float depth = depthTexture.Load(IN.texCoord * windowSize.xy, i);
		float3 normal = normalTexture.Load(IN.texCoord * windowSize.xy, i).rgb * 2.0 - 1.0;

		float3 position = positionTexture.Load(IN.texCoord * windowSize.xy, i).rgb;
		position = position * 2.0 - 1.0;
		position *= 1000.0;

		if (depth == 1.0) {
			color += skydome;
			cosTheta += 1.0;
		}
		else {
			float3 diffuse = colorTexture.Load(IN.texCoord * windowSize.xy, i).rgb;
			diffuse *= max(dot(normal, lightDir), 0.3);

			float4x4 depthBiasMVP = mul(biasMatrix, shadowViewProj);

			float dist = distance(position, camPosition.xyz);
			if (dist > 500.0) {
				diffuse = lerp(skyColor.rgb, diffuse, 1 - clamp((dist - 500.0) / 500.0, 0, 1));
			}

			color += diffuse;
		}
	}
	color /= SAMPLES;

	OUT.color = float4(color, 1.0);

	/*
	if (IN.texCoord.x < 0.25 && IN.texCoord.y < 0.25) {
		float2 coord = IN.texCoord / 0.25;
		float depth = shadowTexture.Sample(texSampler, coord).r;
		float znear = 0.1;
		float zfar = 4096.0;
		float z = (2.0 * znear) / (zfar + znear - depth * (zfar - znear));
		OUT.color = float4(z, z, z, 1.0);
	}
	*/
	return OUT;
};