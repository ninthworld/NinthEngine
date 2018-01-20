struct VertexOut {
	float2 texCoord : TEXCOORD;
	float blend : BLENDWEIGHT;

	float4 position : SV_POSITION;
};

cbuffer Skydome : register(b1) {
	float4 skyColor;
	float4 scale;
};

SamplerState texSampler : register(s0);

Texture2D skyTexture : register(t0);

float4 main(VertexOut IN) : SV_TARGET {
	float4 OUT;

	float4 sky = skyTexture.Sample(texSampler, IN.texCoord);

	OUT = lerp(sky, skyColor, IN.blend);
	
	return OUT;
}