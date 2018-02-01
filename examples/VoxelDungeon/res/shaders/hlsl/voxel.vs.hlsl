
struct VertexIn {
	float3 position : POSITION;
	float3 normal : NORMAL;
	float material : BLENDWEIGHT;
};

struct VertexOut {
	float4 material : TEXCOORD;
	float3 normal : NORMAL;
	float3 position : POSITION;
	float4 svPosition : SV_POSITION;
};

cbuffer Camera : register(b0) {
	float4x4 camViewProj;
	float4 camPosition;
};

cbuffer Chunk : register(b1) {
	float4 chunkPosition;
};

static const float4 Materials[4] = {
	float4(0, 0, 0, 0),				// NULL
	float4(0, 0, 0, 1),				// Stone Floor	[TEXTURE]
	float4(0, 0, 0, 2),				// Stone Wall	[TEXTURE]
	//float4(1, 0, 0, 0),
	//float4(0, 1, 0, 0),
	float4(100, 70, 40, 0) / 255.0	// Stone Wall Pillar Foundation
};

VertexOut main(VertexIn IN) {
	VertexOut OUT;

	OUT.material = Materials[int(IN.material)];
	OUT.normal = (IN.normal + 1.0) / 2.0;
	OUT.position = IN.position + chunkPosition.xyz;
	OUT.svPosition = mul(camViewProj, float4(IN.position + chunkPosition.xyz, 1.0));

	return OUT;
}