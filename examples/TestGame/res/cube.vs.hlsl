
struct input {
	float3 position : POSITION0;
	float3 color : COLOR0;
};

struct output {
	float4 position : SV_POSITION;
	float3 color : COLOR0;
};

cbuffer Camera : register(b0) {
	float4x4 viewMatrix;
	float4x4 viewProjMatrix;
	float4 camPosition;
};

output main(input IN) {
	output OUT;

	OUT.color = IN.color;

	OUT.position = mul(viewProjMatrix, float4(IN.position, 1.0));

	return OUT;
}