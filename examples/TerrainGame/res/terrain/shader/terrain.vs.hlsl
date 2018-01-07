
struct input {
	float2 position : POSITION;
};

struct output {
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

cbuffer Camera : register(b0) {
	matrix viewMatrix;
	matrix viewProjMatrix;
	float4 camPosition;
};

Texture2D heightmap : register(t0);
SamplerState heightmapSampler : register(s0);

output main(input IN) {
	output OUT;
	
	float3 localPos = float3(IN.position.x, 0.0, IN.position.y);

	OUT.texCoord = IN.position;

	float height = 0.0;
	height += heightmap.SampleLevel(heightmapSampler, OUT.texCoord, 0).r;

	localPos.y = height;

	OUT.position = mul(viewProjMatrix, float4(localPos * 16.0, 1.0f));

	return OUT;
}