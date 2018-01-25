
struct VertexIn {
	float3 position : POSITION;
	float3 color : COLOR;
	float3 normal : NORMAL;
};

struct VertexOut {
	float4 svPosition : SV_POSITION;
};

cbuffer ShadowMap : register(b0) {
	float4x4 shadowCamViewProj;
	float4 shadowCamPosition;
};

cbuffer Model : register(b1) {
	float4x4 modelTransform;
};

VertexOut main(VertexIn IN) {
	VertexOut OUT;

	OUT.svPosition = mul(shadowCamViewProj, mul(modelTransform, float4(IN.position, 1.0)));

	return OUT;
}