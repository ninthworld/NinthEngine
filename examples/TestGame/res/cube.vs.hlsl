
struct VertexIn {
	float3 position : POSITION0;
	float3 color : COLOR;
};

struct VertexOut {
	float4 svPosition : SV_POSITION;
	float3 color : COLOR;
};

cbuffer Camera : register(b0) {
	float4x4 camViewProj;
	float4 camPosition;
};

VertexOut main(VertexIn IN) {
	VertexOut OUT;

	OUT.color = IN.color;

	OUT.svPosition = mul(camViewProj, float4(IN.position, 1.0));

	return OUT;
}