
struct VertexIn {
	float2 position : POSITION;
};

struct VertexOut {
	float2 texCoord : TEXCOORD;
	float4 svPosition : SV_POSITION;
};

cbuffer Terrain : register(b1) {
	float4 terrainScale;
};

cbuffer Node : register(b2) {
	float2 nodeLocalPos;
	float nodeScale;
	int nodeLod;
};

SamplerState texSampler : register(s0);

Texture2D heightmap : register(t0);

VertexOut main(VertexIn IN) {
	VertexOut OUT;
	
	float2 localPos = nodeLocalPos + (IN.position * nodeScale);

	float height = heightmap.SampleLevel(texSampler, localPos, 0).r;

	float3 worldPos = float3(localPos.x - 0.5, height, localPos.y - 0.5) * terrainScale.xyz;
	
	OUT.texCoord = localPos;
	OUT.svPosition = float4(worldPos, 1);

	return OUT;
}