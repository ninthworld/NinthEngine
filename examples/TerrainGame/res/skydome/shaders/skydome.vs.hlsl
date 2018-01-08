
struct VertexIn {
	float3 position : POSITION;
	float2 texCoord : TEXCOORD;
};

struct VertexOut {
	float2 texCoord : TEXCOORD;
	float blend : BLENDWEIGHT;

	float4 position : SV_POSITION;
};

cbuffer Camera : register(b0) {
	float4x4 viewMatrix;
	float4x4 viewProjMatrix;
	float4 camPosition;
	float4 frustumPlanes[6];
};

cbuffer Skydome : register(b1) {
	float4 skyColor;
	float4 scale;
};

VertexOut main(VertexIn IN) {
	VertexOut OUT;
	
	OUT.texCoord = IN.texCoord;
	OUT.blend = pow(1.0 - IN.position.y, 2.0);

	OUT.position = mul(viewProjMatrix, float4(IN.position * scale.xyz + camPosition.xyz, 1.0f));

	return OUT;
}