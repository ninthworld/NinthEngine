struct input {
	float2 position : POSITION;
};

struct output {
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
};

cbuffer ViewProjMatrix : register(b0) {
	matrix viewProjMatrix;
};

cbuffer ModelMatrix : register(b1) {
	matrix modelMatrix;
};

Texture2D heightTexture : register(t0);
Texture2D normalTexture : register(t1);
SamplerState heightSampler : register(s0);
SamplerState normalSampler : register(s1);

output main(input IN){
	output OUT;

	float4 pos = mul(modelMatrix, float4(IN.position.x, 0.0, IN.position.y, 1.0));
	float2 adjPos = (pos.xz + 256.0) / 512.0;
	pos.y = heightTexture.SampleLevel(heightSampler, adjPos, 0).r * 64.0;

	OUT.position = mul(viewProjMatrix, pos);
	OUT.normal = normalize(normalTexture.SampleLevel(normalSampler, adjPos, 0).rgb * 2.0 - 1.0);

	return OUT;
}