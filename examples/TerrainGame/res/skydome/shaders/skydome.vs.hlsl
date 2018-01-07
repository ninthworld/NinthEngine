
struct input {
	float3 position : POSITION;
	float2 texCoord : TEXCOORD0;
};

struct output {
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float blend : BLENDWEIGHT0;
};

cbuffer Camera : register(b0) {
	matrix viewMatrix;
	matrix viewProjMatrix;
	float4 camPosition;
};

cbuffer Skydome : register(b1) {
	float4 skyColor;
	float4 scale;
};

output main(input IN) {
	output OUT;
	
	OUT.texCoord = IN.texCoord;
	OUT.blend = pow(1.0 - IN.position.y, 2.0);

	OUT.position = mul(viewProjMatrix, float4(IN.position * scale.xyz + camPosition.xyz, 1.0f));

	return OUT;
}