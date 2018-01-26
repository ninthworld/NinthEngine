
struct VertexIn {
	float3 position : POSITION;
	float3 color : COLOR;
	float3 normal : NORMAL;
};

struct VertexOut {
	float3 color : COLOR;
	float3 normal : NORMAL;
	float4 svPosition : SV_POSITION;
};

cbuffer Camera : register(b0) {
	float4x4 camViewProj;
	float4 camPosition;
};

cbuffer Model : register(b1) {
	float4x4 modelTransform;
};

VertexOut main(VertexIn IN) {
	VertexOut OUT;

	OUT.color = IN.color;
	OUT.normal = (normalize(IN.normal) + 1.0) / 2.0;
	OUT.svPosition = mul(camViewProj, mul(modelTransform, float4(IN.position, 1.0)));

	return OUT;
}