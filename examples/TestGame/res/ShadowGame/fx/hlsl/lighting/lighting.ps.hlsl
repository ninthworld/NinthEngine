
struct VertexOut {
	float2 texCoord : TEXCOORD;
	float4 svPosition : SV_POSITION;
};

struct PixelOut {
	float4 color : SV_TARGET;
};

cbuffer Sky : register(b0) {
	float4 sunPosition;
	float4 skyColor;
};

cbuffer CameraExt : register(b1) {
	float4x4 camView;
	float4x4 camProj;
	float4x4 camInvView;
	float4x4 camInvProj;
};

cbuffer ShadowMap : register(b2) {
	float4x4 shadowCamViewProj;
};

SamplerState texSampler : register(s0);

Texture2D colorTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D<float> depthTexture : register(t2);
Texture2D<float> shadowDepthTexture : register(t3);

float3 worldPosFromDepth(float2 texCoord, float depth) {
	float4 ssPos = mul(camInvProj, float4(texCoord * 2.0 - 1.0, depth, 1.0));
	float4 wsPos = mul(camInvView, ssPos);
	return wsPos.xyz / wsPos.w;
}

float4x4 biasMatrix = float4x4(
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 0.5, 0.0,
	0.5, 0.5, 0.5, 1.0);

PixelOut main(VertexOut IN) {
	PixelOut OUT;
	
	float2 invTexCoord = float2(IN.texCoord.x, 1.0 - IN.texCoord.y);

	float3 lightDir = normalize(sunPosition.xyz);
	
	float3 color = colorTexture.Sample(texSampler, IN.texCoord).rgb;
	float3 normal = normalTexture.Sample(texSampler, IN.texCoord).rgb * 2.0 - 1.0;
	float depth = depthTexture.Sample(texSampler, IN.texCoord).r;
	float3 position = worldPosFromDepth(invTexCoord, depth);

	if (depth != 1.0) {
		float cosTheta = clamp(dot(normal, lightDir), 0, 1);

		float4 shadowCoord = (mul(biasMatrix, mul(shadowCamViewProj, float4(position, 1.0))) + 1.0) / 2.0;
		float visibility = 1.0;
		float shadowDepth = shadowDepthTexture.Sample(texSampler, shadowCoord.xy).r;
		if(shadowDepth < shadowCoord.z - 0.005) {
			visibility = 0.0;
		}

		color *= clamp(visibility, 0.3, 1.0);
	}

	//OUT.color = float4(shadowDepth.xxx, 1);
	OUT.color = float4(color, 1);

	return OUT;
};