
struct VertexIn {
	float3 position : POSITION;
	float2 texCoord : TEXCOORD;
};

struct VertexOut {
	float3 position : POSITION;
	float2 texCoord : TEXCOORD;
	float blend : BLENDWEIGHT;
	float4 svPosition : SV_POSITION;
};

cbuffer Camera : register(b0) {
	float4x4 camViewProj;
	float4 camPosition;
};

VertexOut main(VertexIn IN) {
	VertexOut OUT;

	OUT.position = IN.position;	
	OUT.texCoord = IN.texCoord;
	OUT.blend = clamp(IN.position.y / 0.005, 0, 1);

	OUT.svPosition = mul(camViewProj, float4(IN.position * 400.0 + float3(0, -100.0, 0) + camPosition.xyz, 1.0f));

	return OUT;
}